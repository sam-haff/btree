#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>
#include <stdbool.h>

struct Node{
    struct Node** childs; // dynamic array, to dealloc
    int* keys;
    int childsNum;
    int keysNum;
    int key;
    int isLeaf;
};

struct BTree{
    int _elsNum;
    int _k; // t
    int _maxChildsNum;
    int _maxKeysNum;
    struct Node* _root;
};

// Node "methods"
void removeChild(struct Node* tgt, int ix);
void insertChild(struct Node* tgt, struct Node* c, int ix);
void insertChildSeq(struct Node* tgt, struct Node* c);
int insertKey(struct Node* tgt, int key);
int insertKeySeq(struct Node* tgt, int key);
struct Node* Node_new();
struct Node* allocNode(struct BTree* t);

// BTree "methods"
struct BTree BTree_init(int k);
void _visitInOrder(struct BTree* tree, struct Node* nd, void (*f)(int, void*), void*);
void visitInOrder(struct BTree* tree, void (*f)(int, void*), void*);
void splitNode(struct BTree* tree, struct Node* parent, int child); 
void insert(struct BTree* tree, int key);
void delete(struct BTree* tree, struct Node* rt, int key);
int search(struct BTree* tree, int key); // same as "contains" for now, 1 if tree has key, 0 if not
void freeTree(struct BTree* tree, bool freeRoot);


#endif