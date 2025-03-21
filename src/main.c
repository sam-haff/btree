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

    for (int i = 9; i > 4; i--) {
        delete(&tree, tree._root, i);
        if (tree._root->childsNum == 1 && tree._root->keysNum == 0) {
            tree._root = tree._root->childs[0];
        }
    }

    printf("----Checkpoint :(");
    delete(&tree, tree._root, 4);

    printf("----Checkpoint");
    visitInOrder(&tree, visitF);

    printf("----");

    return 0;
}