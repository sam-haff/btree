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
TEST(tree_tests, tree_init_tree_delete_element_is_not_in_tree_after_deletion) {
    int t = 2;
    struct BTree tree = BTree_init(t);
    insert(&tree, 10);

    EXPECT_EQ(search(&tree, 10), 1);
    delete(&tree, tree._root, 10);
    EXPECT_EQ(search(&tree, 10), 0);
}

TEST(tree_tests, tree_operation_delete_element_is_not_in_tree_after_deletion_large_amount) {
    int t = 2;
    struct BTree tree = BTree_init(t);
#define TEST_KEYS_NUM 40
    for (int i =0; i < TEST_KEYS_NUM; i++) {
        insert(&tree, i);
    }
    for (int i =0; i < TEST_KEYS_NUM; i++) {
        delete(&tree, tree._root, i);
    }
    for (int i = 0; i < TEST_KEYS_NUM; i++) {
        EXPECT_EQ(search(&tree, i), 0);
    }
#undef TEST_KEYS_NUM
}

struct tree_traverse_state{
    int* gathered_keys;
    int capacity;
    int size;
    bool success;
};
struct tree_traverse_state init_tree_traverse_state(int cap) {
    struct tree_traverse_state state;
    state.gathered_keys = malloc(cap*sizeof(int));
    state.size = 0;
    state.success = true;
    state.capacity = cap;
    return state;
}
void free_tree_traverse_state(struct tree_traverse_state s) {
    free(s.gathered_keys);
}
void tree_visit(int key, void* extra) {
    struct tree_traverse_state* s = extra;
    if (s->size == s->capacity) {
        s->success = false;
        return;
    }
    s->gathered_keys[s->size] = key;
    s->size++;
}
TEST(tree_tests, tree_operation_tree_visit_in_order_tree_is_ordered) {
    int t = 2;
    struct BTree tree = BTree_init(t);

#define TEST_KEYS_NUM 10
    int keys[TEST_KEYS_NUM] = {1 ,6, 2, 0, -40, 2, 24, 12, 1, 7};
    int ordered_keys[TEST_KEYS_NUM] = {-40, 0, 1, 1, 2, 2, 6, 7, 12, 24};
    for (int i =0; i < TEST_KEYS_NUM; i++) {
        insert(&tree, keys[i]);
    }

    struct tree_traverse_state s = init_tree_traverse_state(TEST_KEYS_NUM);
    visitInOrder(&tree, tree_visit, &s);

    EXPECT_EQ(s.success, true);
    EXPECT_EQ(s.size, TEST_KEYS_NUM);
    for (int i = 0; i< TEST_KEYS_NUM; i++){
        EXPECT_EQ(ordered_keys[i], s.gathered_keys[i]);
    }
#undef TEST_KEYS_NUM
}


int main(int argc, const char** argv) {
    return rktest_main(argc, argv);
}