#include <stdio.h>
#include <assert.h>
#include "btree.h"

void visitF(int key){
    printf("%d\n", key);
}

int main() {
    struct BTree tree = BTree_init(2);

    //int keys[10] = {1 ,6, 2, 0, -40, 2, 24, 12, 1, 7};
    //for (int i =0; i < 10; i++) {
    //    insert(&tree, keys[i]);
    //}
    //visitInOrder(&tree, visitF);
    for (int i = 0; i < 10; i++){
        insert(&tree, i);
    }

    for (int i = 9; i>2; i--) {
        delete(&tree, tree._root, i);
    }

    visitInOrder(&tree, visitF);

    printf("----");

    return 0;
}