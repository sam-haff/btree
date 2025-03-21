#include <stdio.h>
#include <assert.h>
#include "btree.h"

void visitF(int key, void* extra){
    printf("%d\n", key);
}

int main() {
    struct BTree tree = BTree_init(2);

    int keys[10] = {1 ,6, 2, 0, -40, 2, 24, 12, 1, 7};
    for (int i =0; i < 10; i++) {
        insert(&tree, keys[i]);
    }

    printf("All tree elements in order:\n");
    printf("----\n");
    visitInOrder(&tree, visitF, NULL);

    for (int i = 9; i >= 0; i--) {
        delete(&tree, tree._root, keys[i]);
    }
    printf("----\n\n");

    printf("Tree elements after deletion:(should be empty)\n");
    printf("----\n");
    visitInOrder(&tree, visitF, NULL);

    printf("----\n");

    return 0;
}