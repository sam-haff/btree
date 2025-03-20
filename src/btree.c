#include "btree.h"
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
void _visitInOrder(struct BTree* tree, struct Node* nd, void (*f)(int)) {
    if (!nd->isLeaf) {
        for (int i = 0; i < nd->childsNum; i++) {
            _visitInOrder(tree, nd->childs[i], f);
            if (i != nd->childsNum - 1)
                f(nd->keys[i]);
        }
    }
    else {
        for (int i = 0; i < nd->keysNum; i++)
            f(nd->keys[i]);
    }
}
void visitInOrder(struct BTree* tree, void (*f)(int)) {
    _visitInOrder(tree, tree->_root, f);
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

