#include <stdio.h>
#include "btree.h"

void visitF(int key){
    printf("%d\n", key);
}

int main() {
    struct BTree tree = BTree_init(2);
    insert(&tree, 1);
    insert(&tree, 2);
    insert(&tree, 3);
    insert(&tree, 6);
    insert(&tree, 4);
    insert(&tree, 5);
    insert(&tree, 11);
    insert(&tree, -10);
    insert(&tree, 50); 

    insert(&tree, 0);
    insert(&tree, -1);
    insert(&tree, 2); 
    visitInOrder(&tree, visitF);

    return 0;
}