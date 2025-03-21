//#include "minunit.h"
#include <windows.h>
#define DEFINE_RKTEST_IMPLEMENTATION
#include "rktest.h"

#include "../src/btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


TEST(tree_tests, tree_init_max_keys_constraint_computed_correctly) {
    int t = 2;
    struct BTree tree = BTree_init(t);

    EXPECT_EQ(tree._maxKeysNum, 3);
}

TEST(tree_tests, tree_init_max_childs_contsraint_computed_correctly) {
    int t = 2;
    struct BTree tree = BTree_init(t);

    EXPECT_EQ(tree._maxChildsNum, 4);
}

TEST(tree_tests, tree_init_tree_is_empty) {
    int t = 2;
    struct BTree tree = BTree_init(t);

    EXPECT_EQ(search(&tree, 1), 0);
}

TEST(tree_tests, tree_init_tree_insertion_inserted_element_is_present_in_tree) {
    int t = 2;
    struct BTree tree = BTree_init(t);
    insert(&tree, 1);
    EXPECT_EQ(search(&tree, 1), 1);
}

TEST(tree_tests, tree_init_tree_insertion_multiple_inserted_elements_are_present_in_tree) {
    int t = 2;
    struct BTree tree = BTree_init(t);
#define TEST_KEYS_NUM 10
    int keys[TEST_KEYS_NUM] = {1 ,6, 2, 0, -40, 2, 24, 12, 1, 7};
    for (int i =0; i < TEST_KEYS_NUM; i++) {
        insert(&tree, keys[i]);
    }
    for (int i = 0; i < TEST_KEYS_NUM; i++) {
        EXPECT_EQ(search(&tree, keys[i]), 1);
    }
#undef TEST_KEYS_NUM
}
TEST(tree_tests, tree_init_tree_insertion_large_amount_inserted_elements_are_present_in_tree) {
    int t = 2;
    struct BTree tree = BTree_init(t);
#define TEST_KEYS_NUM 40
    for (int i =0; i < TEST_KEYS_NUM; i++) {
        insert(&tree, i);
    }
    for (int i = 0; i < TEST_KEYS_NUM; i++) {
        EXPECT_EQ(search(&tree, i), 1);
    }
#undef TEST_KEYS_NUM
}

int main(int argc, const char** argv) {
    return rktest_main(argc, argv);
}