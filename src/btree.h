#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>

struct Node{
    struct Node** childs; // dynamic array, to dealloc
    int* keys;
    int childsNum;
    int keysNum;
    int key;
    int isLeaf;
};

struct BTree{
    int _k;
    int _maxChildsNum;
    int _maxKeysNum;
    struct Node* _root;
};

// Node "methods"
void insertChild(struct Node* tgt, struct Node* c, int ix);
void insertChildSeq(struct Node* tgt, struct Node* c);
int insertKey(struct Node* tgt, int key);
int insertKeySeq(struct Node* tgt, int key);
struct Node* Node_new();
struct Node* allocNode(struct BTree* t);

// BTree "methods"
struct BTree BTree_init(int k);
void _visitInOrder(struct BTree* tree, struct Node* nd, void (*f)(int));
void visitInOrder(struct BTree* tree, void (*f)(int));
void splitNode(struct BTree* tree, struct Node* parent, int child); 
void insert(struct BTree* tree, int key);


#endif