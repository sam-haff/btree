#include "btree.h"
#include <stdbool.h>
#include <assert.h>

// insert child at the requested index
void insertChild(struct Node* tgt, struct Node* c, int ix) {
    if (ix == tgt->childsNum) {
        tgt->childs[ix] = c;
        tgt->childsNum++;
        return;
    }
    // invariant: childsNum < k strict
    int i = tgt->childsNum - 1;
    while (i>=ix) {
        tgt->childs[i+1] = tgt->childs[i];
        i--;
    }
    i++;
    tgt->childs[ix] = c;
    tgt->childsNum++;
}
// insert child at the end
void insertChildSeq(struct Node* tgt, struct Node* c) {
    insertChild(tgt, c, tgt->childsNum);
}
// insert key maintaining sorted order
int insertKey(struct Node* tgt, int key) {
    int i = tgt->keysNum-1;
    while (i>=0 && tgt->keys[i] > key){
        tgt->keys[i+1] = tgt->keys[i];
        i--;
    }
    i++;
    tgt->keys[i] = key;
    tgt->keysNum++;
    
    return i;
}
// insert key at the end of the array
int insertKeySeq(struct Node* tgt, int key) {
    tgt->keys[tgt->keysNum] = key;
    tgt->keysNum++;
    return tgt->keysNum-1;
}


struct Node* Node_new(){
    struct Node* nd = (struct Node*)malloc(sizeof(struct Node));
    nd->childsNum = 0;
    nd->keysNum = 0; 
    nd->childs = 0; 
    nd->isLeaf = 0; // false
    nd->key = 0;
}


struct Node* allocNode(struct BTree* t) {
    struct Node* nd = Node_new();

    nd->childs = (struct Node**)malloc(sizeof(void*) * t->_maxChildsNum);
    nd->keys = (int*)malloc(sizeof(int)*t->_maxKeysNum);

    return nd;
}
void freeNode(struct Node* nd){
    free(nd->childs);
    free(nd->keys);
    free(nd);
}
void removeChildPtr(struct Node* tgt, struct Node* nd) {
    for (int i = 0; i < tgt->childsNum; i++) {
        if (tgt->childs[i]==nd) {
            removeChild(tgt, i);
            return;
        }
    }
}
void freeTree(struct BTree* tree, bool freeRoot) {
    // dfs
    int estimatedMaxNodes = tree->_elsNum/(tree->_k-1);
    struct Node** st = malloc(estimatedMaxNodes * sizeof(struct Node*));
    int stPtr = 0;

    st[stPtr++] = tree->_root;
    while (stPtr != 0) {
        struct Node* parent = stPtr > 1 ? st[stPtr-2] : NULL ;
        struct Node* curr = st[stPtr-1];
        if (curr->childsNum == 0 || curr->isLeaf) {
            if (stPtr > 1) {
                removeChildPtr(parent, curr);
                freeNode(curr);
            }
            if (stPtr == 1) {
                while (curr->keysNum) {
                    removeKey(curr, 0);
                }
            }
            stPtr--;
            continue;
        }
        st[stPtr] = st[stPtr-1]->childs[0];
        stPtr++;
    }

    tree->_elsNum = 0;

    if (freeRoot) {
        freeNode(tree->_root);
        tree->_root = NULL;
    }
    free(st);
}

struct BTree BTree_init(int k) {
    assert(k >= 2);

    struct BTree tree;
    tree._k = k;
    tree._maxChildsNum = 2*k;
    tree._maxKeysNum = 2*k - 1;

    tree._root = allocNode(&tree);
    tree._root->isLeaf = 1;

    return tree;
}
void _visitInOrder(struct BTree* tree, struct Node* nd, void (*f)(int, void*), void* extra) {
    if (!nd->isLeaf) {
        for (int i = 0; i < nd->childsNum; i++) {
            _visitInOrder(tree, nd->childs[i], f, extra);
            if (i != nd->childsNum - 1)
                f(nd->keys[i], extra);
        }
    }
    else {
        for (int i = 0; i < nd->keysNum; i++)
            f(nd->keys[i], extra);
    }
}
void visitInOrder(struct BTree* tree, void (*f)(int, void*), void* extra) {
    _visitInOrder(tree, tree->_root, f, extra);
}

void splitNode(struct BTree* tree, struct Node* parent, int child) {
        parent->isLeaf = 0;
        struct Node* ch = parent->childs[child];

        struct Node* newNd = allocNode(tree);
        newNd->isLeaf = ch->isLeaf;
        
        int j = 0;
        for (int i = tree->_k; i < tree->_maxKeysNum; i++) {
            insertKeySeq(newNd, ch->keys[i]);
        }
        for (int i = tree->_k; i < tree->_maxChildsNum; i++) {
            insertChild(newNd, ch->childs[i], j);
            j++;
        }

        int keyIx = insertKey(parent, ch->keys[tree->_k-1]);
        insertChild(parent, newNd, keyIx+1);

        ch->childsNum = tree->_k;
        ch->keysNum = tree->_k - 1;
    }
int search(struct BTree* tree, int key) {
    struct Node* curr = tree->_root;

    while (!curr->isLeaf) {
        int i = curr->keysNum - 1; 
        while (i >= 0 && curr->keys[i] > key) i--;
        if (i >= 0 && curr->keys[i] == key) { return 1; }
        i++;
        
        curr = curr->childs[i];
    }
    for (int i = 0; i < curr->keysNum; i++) {
        if (curr->keys[i] == key)
            return 1;
    }
    return 0;
}
void insert(struct BTree* tree, int key) {
    tree->_elsNum++;

    // traverse state
    struct Node* curr = tree->_root;
    int currIxTr[12];
    int currIxPos = 0;
    currIxTr[0] = 0;
    struct Node* parentTr[12];
    parentTr[0] = (struct Node*)0;

    while (1) {
        int currIx = currIxTr[currIxPos];
        struct Node* parent = parentTr[currIxPos];

        // if node is full - split
        if (curr->keysNum == tree->_maxKeysNum) {
            if (!parent){
                parent = allocNode(tree);
                insertChildSeq(parent, curr); 

                tree->_root = parent;
            }
            splitNode(tree, parent, currIx);

            // step back.
            // step back is needed because we are splitting the node we already descended into,
            // and after split it could not be the node we needed to descend into(we might need
            // to descend into a new after-split node instead).
            curr = parent;
            if (currIxPos !=0 ) 
                currIxPos--;
            else {
                currIxTr[0] = 0;
                parentTr[0] = (struct Node*)0; 
            }
            continue;
        }

        if (curr->isLeaf) {
            insertKey(curr, key);
            break;
        }

        // search child to descend into
        int i = curr->keysNum - 1; 
        while (i >= 0 && curr->keys[i] > key) i--;

        i++;

        // descend and update stack(needed because we do step back in case of split).
        
        parent = curr;
        curr = curr->childs[i];
        currIx = i;

        currIxPos++;

        parentTr[currIxPos] = parent;
        currIxTr[currIxPos] = currIx;
    }
}

void removeChild(struct Node* tgt, int ix){
    assert(tgt->childsNum > 0);

    for (int i = ix; i < tgt->childsNum-1; i++) {
        tgt->childs[i] = tgt->childs[i+1];
    }
    tgt->childsNum--;
}
void removeKey(struct Node* tgt, int ix){
    assert(tgt->keysNum > 0);

    for (int i = ix; i < tgt->keysNum-1; i++) {
        tgt->keys[i] = tgt->keys[i+1];
    }
    tgt->keysNum--;
}

void delete(struct BTree* tree, struct Node* rt, int key) {
    tree->_elsNum--;

    struct Node* curr = rt;

    while (!curr->isLeaf) {
        int i = curr->keysNum - 1; 
        while (i >= 0 && curr->keys[i] > key) i--;
        if (i >= 0 && curr->keys[i] == key) {
            // we cant simply delete k from keys in the node because we then will need to do smth with extra child.
            // so instead we are doing the following procedure.

            if (curr->childs[i]->keysNum >= tree->_k) {
               
                // keys[i] is k.
                // ascend last key from left child to keys[i].
                // now there no target key k in the tree.
                // but we have duplicate key in ch[i] so we need to now delete it.
                // => change target key to that duplicate key and recurse.
                int _key = curr->childs[i]->keys[curr->childs[i]->keysNum-1];
                
                curr->keys[i] = _key;
                curr = curr->childs[i];
                key = _key;
                continue; 
            } else
            if (curr->childs[i+1]->keysNum >= tree->_k) {
                // same as prev symmetrically for right child.
                int _key = curr->childs[i+1]->keys[0];
                
                curr->keys[i] = _key;
                curr = curr->childs[i+1];
                key = _key;
                continue; 

            } else {
                // remove k in curr.
                // merge keys as ch[i].keys[0],...,ch[i].keys[last], k, ch[i+1].keys[0],...,ch[i+1].keys[last] in ch[i].
                // merge childs as ch[i].ch[0],...,ch[i].ch[last], ch[i+1].ch[0], ..., ch[i+1].ch[last] in ch[i].
                // remove ch[i+1].
                // recurse to ch[i+1].
                removeKey(curr, i);
                for (int j = 0; j < curr->childs[i+1]->childsNum; j++) {
                    insertChildSeq(curr->childs[i], curr->childs[i+1]->childs[j]);
                }
                insertKeySeq(curr->childs[i], key);
                for (int j = 0; j < curr->childs[i+1]->keysNum; j++) {
                    insertKeySeq(curr->childs[i], curr->childs[i+1]->keys[j]);
                }
                struct Node* nd = curr->childs[i+1];
                removeChild(curr, i+1);
                freeNode(nd); // node is merged and no longer referenced

                curr = curr->childs[i];

                continue;
            }
            return; 
        }
        i++; // now this is the index of the child that potentially contains target key 

        // remember the property of BTree - all inner nodes should contain at least t-1 keys.
        // that means we can't do deletion op on the node that already has t-1 keys.
        // so before descending to a node, we need to ensure it has at least t keys(if we delete from it, 
        // then we at least still have valid node).
        if (curr->childs[i]->keysNum == tree->_k - 1) {
            // keys number is not sufficient, need to cover that gap
            if (i > 0 && curr->childs[i-1]->keysNum >= tree->_k) {
                // left child(lch) is ch[i-1], right(rch) is ch[i].
                // left key is keys[i-1].
                // lch, keys[i-1], rch

                // take left key, and save it.
                // ascend rightmost key from left child to the left key(replace it).
                // descend saved old left key to right child(we be a new rch->keys[0]).
                // remove rightmost key from left child.

                // descend key from curr to child i
                int keyToDesc = curr->keys[i-1];

                curr->keys[i-1] = curr->childs[i - 1]->keys[curr->childs[i-1]->keysNum-1];
                insertKey(curr->childs[i], keyToDesc); 

                removeKey(curr->childs[i-1], curr->childs[i-1]->keysNum-1); 

                struct Node* opChild = curr->childs[i-1];
                if (!opChild->isLeaf) {
                    struct Node* childToMove = opChild->childs[opChild->childsNum-1];
                    removeChild(opChild, opChild->childsNum - 1);
                    insertChild(curr->childs[i], childToMove, 0); 
                }
            } else
            if (i < curr->childsNum-1 && curr->childs[i+1]->keysNum >= tree->_k) {
                // same as prev, symetrically for right key.
                int keyToDesc = curr->keys[i]; 

                curr->keys[i] = curr->childs[i + 1]->keys[0];
                insertKey(curr->childs[i], keyToDesc); 

                removeKey(curr->childs[i+1], 0); 

                struct Node* opChild = curr->childs[i+1];
                if (!opChild->isLeaf) {
                    struct Node* childToMove = opChild->childs[0];
                    removeChild(opChild, 0);
                    insertChildSeq(curr->childs[i], childToMove); 
                }
            }
            else {
                // both siblings have not enough keys to make a donation to our descend candidate node.
                // => merge arbitrary sibling to our candidate.

                if (i > 0) {
                    // merge our candate(ch[i]) to left sibling(ch[i-1]).
                    // take left sibling(ch[i-1]).
                    // save left key(keys[i-1]), call it lkey.
                    // merge left sibling keys with candidate as 
                    // ch[i-1].keys[0]...ch[i-1].keys[last], lkey, ch[i].keys[0],...,ch[i].keys[last].
                    // merge left sibling childs with candidate as
                    // ch[i-1].ch[0],..., ch[i-1].ch[last], ch[i].ch[0],...,ch[i].ch[last].
                    // remove our candidate as it is merged into left siblint and is no longer useful.
                    // make left sibling our new candidate.
                    struct Node* left = curr->childs[i-1];
                    insertKey(left, curr->keys[i-1]); 
                    for (int j = 0; j< curr->childs[i]->keysNum; j++) {
                        insertKeySeq(left, curr->childs[i]->keys[j]);
                    }
                    if (!curr->childs[i-1]->isLeaf)
                        for (int j = 0; j < curr->childs[i]->childsNum; j++) {
                            insertChildSeq(curr->childs[i-1], curr->childs[i]->childs[j]);
                        }
                    removeKey(curr, i-1);
                    struct Node* nd = curr->childs[i];
                    removeChild(curr, i); 
                    freeNode(nd); // node is merged and no longer referenced
                    i = i - 1;
                } else { 
                    // same as prev, symetrically for right sibling.
                    struct Node* right = curr->childs[i+1];
                    insertKey(curr->childs[i], curr->keys[i]); 
                    for (int j = 0; j< right->keysNum; j++) {
                        insertKeySeq(curr->childs[i], right->keys[j]);
                    }
                    if (!curr->childs[i]->isLeaf)
                        for (int j = 0; j < right->childsNum; j++) {
                            insertChildSeq(curr->childs[i], right->childs[j]);
                        }
                    removeKey(curr, i);
                    removeChild(curr, i+1); 
                    freeNode(right); // node is merged and out of the tree
                }
            }
        }
        curr = curr->childs[i];
    }

    // on leaf -> simply delete the key
    for (int i = 0; i < curr->keysNum; i++){
        if (curr->keys[i] == key) {
            removeKey(curr, i);
            break;
        }
    }

    // shrink if needed
    if (tree->_root->childsNum == 1 && tree->_root->keysNum == 0) {
        struct Node* nd = tree->_root;
        tree->_root = tree->_root->childs[0];
        free(nd);
    }
}

