Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 40: Binary Search Trees

The binary tree is the simplest tree based data structure and while it has been replaced by Hash Maps in many languages is still useful for many applications. Variants on the binary tree exist for very useful things like database indexes, search algorithm structures, and even graphics processing.

I'm calling my binary tree a BSTree for "binary search tree" and the best way to describe it is that it's another way to do a Hashmap style key/value store. The difference is that instead of hashing the key to find a location, the BSTree compares the key to nodes in a tree, and then walks the tree to find the best place to store it based on how it compares to other nodes.

Before I really explain how this works, let me show you the bstree.h header file so you can see the data structures, then I can use that to explain how it's built.
    
    #ifndef _lcthw_BSTree_h
    #define _lcthw_BSTree_h
    
    
    typedef int (*BSTree_compare)(void *a, void *b);
    
    typedef struct BSTreeNode {
        void *key;
        void *data;
    
        struct BSTreeNode *left;
        struct BSTreeNode *right;
        struct BSTreeNode *parent;
    } BSTreeNode;
    
    typedef struct BSTree {
        int count;
        BSTree_compare compare;
        BSTreeNode *root;
    } BSTree;
    
    typedef int (*BSTree_traverse_cb)(BSTreeNode *node);
    
    BSTree *BSTree_create(BSTree_compare compare);
    void BSTree_destroy(BSTree *map);
    
    int BSTree_set(BSTree *map, void *key, void *data);
    void *BSTree_get(BSTree *map, void *key);
    
    int BSTree_traverse(BSTree *map, BSTree_traverse_cb traverse_cb);
    
    void *BSTree_delete(BSTree *map, void *key);
    
    #endif
    

This follows the same pattern I've been using this whole time where I have a base "container" named BSTree and that then has nodes names BSTreeNode that make up the actual contents. Bored yet? Good, there's no reason to be clever with this kind of structure.

The important part is how the BSTreeNode is configured and how it gets used to do each operation: set, get, and delete. I'll cover get first since it's the easiest operation and I'll pretend I'm doing it manually against the data structure:

   I take the key you're looking for and I start at the root. First thing I do is compare your key with that node's key.
   If your key is less-than the node.key, then I traverse down the tree using the left pointer.
   If your key is greater-than the node.key, then I go down with right.
   I repeat step 2 and 3 until either I find a matching node.key, or I get to a node that has no left and right. In the first case I return the node.data, in the second I return NULL.

That's all there is to get, so now to do set it's nearly the same thing, except you're looking for where to put a new node:

   If there is no BSTree.root then I just make that and we're done. That's the first node.
   After that I compare your key to node.key, starting at the root.
   If your key is less-than or equal to the node.key then I want to go left. If your key is greater-than (not equal) then I want to go right.
   I keep repeating 3 until I reach a node where the left or right doesn't exist, but that's the direction I need to go.
   Once there I set that direction (left or right) to a new node for the key and data I want, and set this new node's parent to the previous node I came from. I'll use the parent node when I do delete.

This also makes sense given how get works. If finding a node involves going left or right depending on how they key compares, well then setting a node involves the same thing until I can set the left or right for a new node.

Take some time to draw out a few trees on paper and go through some setting and getting nodes so you understand how it work. After that you are ready to look at the implementation so that I can explain delete. Deleting in trees is a major pain, and so it's best explained by doing a line-by-line code breakdown.
    
    #include <lcthw/dbg.h>
    #include <lcthw/bstree.h>
    #include <stdlib.h>
    #include <lcthw/bstrlib.h>
    
    static int default_compare(void *a, void *b)
    {
        return bstrcmp((bstring)a, (bstring)b);
    }
    
    
    BSTree *BSTree_create(BSTree_compare compare)
    {
        BSTree *map = calloc(1, sizeof(BSTree));
        check_mem(map);
    
        map->compare = compare == NULL ? default_compare : compare;
    
        return map;
    
    error:
        if(map) {
            BSTree_destroy(map);
        }
        return NULL;
    }
    
    static int BSTree_destroy_cb(BSTreeNode *node)
    {
        free(node);
        return 0;
    }
    
    void BSTree_destroy(BSTree *map)
    {
        if(map) {
            BSTree_traverse(map, BSTree_destroy_cb);
            free(map);
        }
    }
    
    
    static inline BSTreeNode *BSTreeNode_create(BSTreeNode *parent, void *key, void *data)
    {
        BSTreeNode *node = calloc(1, sizeof(BSTreeNode));
        check_mem(node);
    
        node->key = key;
        node->data = data;
        node->parent = parent;
        return node;
    
    error:
        return NULL;
    }
    
    
    static inline void BSTree_setnode(BSTree *map, BSTreeNode *node, void *key, void *data)
    {
        int cmp = map->compare(node->key, key);
    
        if(cmp <= 0) {
            if(node->left) {
                BSTree_setnode(map, node->left, key, data);
            } else {
                node->left = BSTreeNode_create(node, key, data);
            }
        } else {
            if(node->right) {
                BSTree_setnode(map, node->right, key, data);
            } else {
                node->right = BSTreeNode_create(node, key, data);
            }
        }
    }
    
    
    int BSTree_set(BSTree *map, void *key, void *data)
    {
        if(map->root == NULL) {
            // first so just make it and get out
            map->root = BSTreeNode_create(NULL, key, data);
            check_mem(map->root);
        } else {
            BSTree_setnode(map, map->root, key, data);
        }
    
        return 0;
    error:
        return -1;
    }
    
    static inline BSTreeNode *BSTree_getnode(BSTree *map, BSTreeNode *node, void *key)
    {
        int cmp = map->compare(node->key, key);
    
        if(cmp == 0) {
            return node;
        } else if(cmp < 0) {
            if(node->left) {
                return BSTree_getnode(map, node->left, key);
            } else {
                return NULL;
            }
        } else {
            if(node->right) {
                return BSTree_getnode(map, node->right, key);
            } else {
                return NULL;
            }
        }
    }
    
    void *BSTree_get(BSTree *map, void *key)
    {
        if(map->root == NULL) {
            return NULL;
        } else {
            BSTreeNode *node = BSTree_getnode(map, map->root, key);
            return node == NULL ? NULL : node->data;
        }
    }
    
    
    static inline int BSTree_traverse_nodes(BSTreeNode *node, BSTree_traverse_cb traverse_cb)
    {
        int rc = 0;
    
        if(node->left) {
            rc = BSTree_traverse_nodes(node->left, traverse_cb);
            if(rc != 0) return rc;
        }
    
        if(node->right) {
            rc = BSTree_traverse_nodes(node->right, traverse_cb);
            if(rc != 0) return rc;
        }
    
        return traverse_cb(node);
    }
    
    int BSTree_traverse(BSTree *map, BSTree_traverse_cb traverse_cb)
    {
        if(map->root) {
            return BSTree_traverse_nodes(map->root, traverse_cb);
        }
    
        return 0;
    }
    
    static inline BSTreeNode *BSTree_find_min(BSTreeNode *node)
    {
        while(node->left) {
            node = node->left;
        }
    
        return node;
    }
    
    static inline void BSTree_replace_node_in_parent(BSTree *map, BSTreeNode *node, BSTreeNode *new_value)
    {
        if(node->parent) {
            if(node == node->parent->left) {
                node->parent->left = new_value;
            } else {
                node->parent->right = new_value;
            }
        } else {
            // this is the root so gotta change it
            map->root = new_value;
        }
    
        if(new_value) {
            new_value->parent = node->parent;
        }
    }
    
    static inline void BSTree_swap(BSTreeNode *a, BSTreeNode *b)
    {
        void *temp = NULL;
        temp = b->key; b->key = a->key; a->key = temp;
        temp = b->data; b->data = a->data; a->data = temp;
    }
    
    static inline BSTreeNode *BSTree_node_delete(BSTree *map, BSTreeNode *node, void *key)
    {
        int cmp = map->compare(node->key, key);
    
        if(cmp < 0) {
            if(node->left) {
                return BSTree_node_delete(map, node->left, key);
            } else {
                // not found
                return NULL;
            }
        } else if(cmp > 0) {
            if(node->right) {
                return BSTree_node_delete(map, node->right, key);
            } else {
                // not found
                return NULL;
            }
        } else {
            if(node->left && node->right) {
                // swap this node for the smallest node that is bigger than us
                BSTreeNode *successor = BSTree_find_min(node->right);
                BSTree_swap(successor, node);
    
                // this leaves the old successor with possibly a right child
                // so replace it with that right child
                BSTree_replace_node_in_parent(map, successor, successor->right);
    
                // finally it's swapped, so return successor instead of node
                return successor;
            } else if(node->left) {
                BSTree_replace_node_in_parent(map, node, node->left);
            } else if(node->right) {
                BSTree_replace_node_in_parent(map, node, node->right);
            } else {
                BSTree_replace_node_in_parent(map, node, NULL);
            }
    
            return node;
        }
    }
    
    void *BSTree_delete(BSTree *map, void *key)
    {
        void *data = NULL;
    
        if(map->root) {
            BSTreeNode *node = BSTree_node_delete(map, map->root, key);
    
            if(node) {
                data = node->data;
                free(node);
            }
        }
    
        return data;
    }
    

Before getting into how BSTree_delete works I want to explain a pattern I'm using for doing recursive function calls in a sane way. You'll find that many tree based data structures are easy to write if you use recursion, but that forumlating a single recursive function is difficult. Part of the problem is that you need to setup some initial data for the first operation, then recurse into the data structure, which is hard to do with one function.

The solution is to use two functions. One function "sets up" the data structure and initial recursion conditions so that a second function can do the real work. Take a look at BSTree_get first to see what I mean:

   I have an initial condition to handle that if map->root is NULL then return NULL and don't recurse.
   I then setup a call to the real recursion, which is in BSTree_getnode. I create the initial condition of the root node to start with, the key, and the map.
   In the BSTree_getnode then I do the actual recursive logic. I compare the keys with map->compare(node->key, key) and go left, right, or equal depending on that.
   Since this function is "self-similar" and doesn't have to handle any initial conditions (because BSTree_get did) then I can structure it verys simply. When it's done it returns to the caller, and that return then comes back to BSTree_get the result.
   At the end, the BSTree_get then handles getting the node.data element but only if the result isn't NULL.

This way of structuring a recursive algorithm matches the way I structure my recursive data structures. I have an initial "base function" that handles initial conditions and some edge cases, then it calls a clean recursive function that does the work. Compare that with how I have a "base struct" in BStree combined with recursive BSTreeNode structures that all reference each other in a tree. Using this pattern makes it easy to deal with recursion and keep it straight.

Next, go look at BSTree_set and BSTree_setnode to see the exact same pattern going on. I use BSTree_set to configure the initial conditions and edge cases. A common edge case is that there's no root node, so I have to make one to get things started.

This pattern will work with nearly any recursive algorithm you have to figure out. The way I do this is I follow this pattern:

   Figure out the initial variables, how they change, and what the stopping conditions are for each recursive step.
   Write a recursive function that calls itself, with arguments for each stopping condition and initial variable.
   Write a setup function to set initial starting conditions for the algorithm and handle edge cases, then it calls the recursive function.
   Finally, the setup function returns the final result and possibly alters it if the recursive function can't handle final edge cases.

Which leads me finally to BSTree_delete and BSTree_node_delete. First you can just look at BSTree_delete and see that it's the setup function, and what it is doing is grabbing the resulting node data and freeing the node that's found. In BSTree_node_delete is where things get complex because to delete a node at any point in the tree, I have to rotate that node's children up to the parent. I'll break this function down and the ones it uses:

bstree.c:190
    I run the compare function to figure out which direction I'm going.
bstree.c:192-198
    This is the usual "less-than" branch where I want to go left. I'm handling the case that left doesn't exist here and returning NULL to say "not found". This handles deleting something that isn't in the BSTree.
bstree.c:199-205
    The same thing but for the right branch of the tree. Just keep recursing down into the tree just like in the other functions, and return NULL if it doesn't exist.
bstree.c:206
    This is where I have found the node since the key is equal (compare return 0).
bstree.c:207
    This node has both a left and right branch, so it's deeply embedded in the tree.
bstree.c:209
    To remove this node I need to first find the smallest node that's greater than this node, which means I call BSTree_find_min on the right child.
bstree.c:210
    Once I have this node, I will do a swap on its key and data with the current node's. This will effectively take this node that was down at the bottom of the tree, and put it's contents here so that I don't have to try and shuffle this node out by its pointers.
bstree.c:214
    The successor is now this dead branch that has the current node's values. It could just be removed, but there's a chance that it has a right node value, which means I need to do a single rotate so that the successor's right node gets moved up to completely detach it.
bstree.c:217
    At this point, the successor is removed from the tree, its values replaced the current node's values, and any children it had are moved up into the parent. I can return the successor as if it were the node.
bstree.c:218
    At this branch I know that the node has a left but no right, so I want to replace this node with its left child.
bstree.c:219
    I again use BSTree_replace_node_in_parent to do the replace, rotating the left child up.
bstree.c:220
    This branch of the if-statement means I have a right child but no left child, so I want to rotate the right child up.
bstree.c:221
    Again, use the function to do the rotate, but this time of the right node.
bstree.c:222
    Finally, the only thing that's left is the condition that I've found the node, and it has no children (no left or right). In this case, I simply replace this node with NULL using the same function I did with all the others.
bstree.c:210
    After all that, I have the current node rotated out of the tree and replaced with some child element that will fit in the tree. I just return this to the caller so it can be freed and managed.

This operation is very complex, and to be honest, in some tree data structures I just don't bother doing deletes and treat them like constant data in my software. If I need to do heavy insert and delete, I use a Hashmap instead.

Finally, you can look at the unit test to see how I'm testing it:
    
    #include "minunit.h"
    #include <lcthw/bstree.h>
    #include <assert.h>
    #include <lcthw/bstrlib.h>
    #include <stdlib.h>
    #include <time.h>
    
    BSTree *map = NULL;
    static int traverse_called = 0;
    struct tagbstring test1 = bsStatic("test data 1");
    struct tagbstring test2 = bsStatic("test data 2");
    struct tagbstring test3 = bsStatic("xest data 3");
    struct tagbstring expect1 = bsStatic("THE VALUE 1");
    struct tagbstring expect2 = bsStatic("THE VALUE 2");
    struct tagbstring expect3 = bsStatic("THE VALUE 3");
    
    static int traverse_good_cb(BSTreeNode *node)
    {
        debug("KEY: %s", bdata((bstring)node->key));
        traverse_called++;
        return 0;
    }
    
    
    static int traverse_fail_cb(BSTreeNode *node)
    {
        debug("KEY: %s", bdata((bstring)node->key));
        traverse_called++;
    
        if(traverse_called == 2) {
            return 1;
        } else {
            return 0;
        }
    }
    
    
    char *test_create()
    {
        map = BSTree_create(NULL);
        mu_assert(map != NULL, "Failed to create map.");
    
        return NULL;
    }
    
    char *test_destroy()
    {
        BSTree_destroy(map);
    
        return NULL;
    }
    
    
    char *test_get_set()
    {
        int rc = BSTree_set(map, &test1, &expect1);
        mu_assert(rc == 0, "Failed to set &test1");
        bstring result = BSTree_get(map, &test1);
        mu_assert(result == &expect1, "Wrong value for test1.");
    
        rc = BSTree_set(map, &test2, &expect2);
        mu_assert(rc == 0, "Failed to set test2");
        result = BSTree_get(map, &test2);
        mu_assert(result == &expect2, "Wrong value for test2.");
    
        rc = BSTree_set(map, &test3, &expect3);
        mu_assert(rc == 0, "Failed to set test3");
        result = BSTree_get(map, &test3);
        mu_assert(result == &expect3, "Wrong value for test3.");
    
        return NULL;
    }
    
    char *test_traverse()
    {
        int rc = BSTree_traverse(map, traverse_good_cb);
        mu_assert(rc == 0, "Failed to traverse.");
        mu_assert(traverse_called == 3, "Wrong count traverse.");
    
        traverse_called = 0;
        rc = BSTree_traverse(map, traverse_fail_cb);
        mu_assert(rc == 1, "Failed to traverse.");
        mu_assert(traverse_called == 2, "Wrong count traverse for fail.");
    
        return NULL;
    }
    
    char *test_delete()
    {
        bstring deleted = (bstring)BSTree_delete(map, &test1);
        mu_assert(deleted != NULL, "Got NULL on delete.");
        mu_assert(deleted == &expect1, "Should get test1");
        bstring result = BSTree_get(map, &test1);
        mu_assert(result == NULL, "Should delete.");
    
        deleted = (bstring)BSTree_delete(map, &test1);
        mu_assert(deleted == NULL, "Should get NULL on delete");
    
        deleted = (bstring)BSTree_delete(map, &test2);
        mu_assert(deleted != NULL, "Got NULL on delete.");
        mu_assert(deleted == &expect2, "Should get test2");
        result = BSTree_get(map, &test2);
        mu_assert(result == NULL, "Should delete.");
    
        deleted = (bstring)BSTree_delete(map, &test3);
        mu_assert(deleted != NULL, "Got NULL on delete.");
        mu_assert(deleted == &expect3, "Should get test3");
        result = BSTree_get(map, &test3);
        mu_assert(result == NULL, "Should delete.");
    
        // test deleting non-existent stuff
        deleted = (bstring)BSTree_delete(map, &test3);
        mu_assert(deleted == NULL, "Should get NULL");
    
        return NULL;
    }
    
    char *test_fuzzing()
    {
        BSTree *store = BSTree_create(NULL);
        int i = 0;
        int j = 0;
        bstring numbers[100] = {NULL};
        bstring data[100] = {NULL};
        srand((unsigned int)time(NULL));
    
        for(i = 0; i < 100; i++) {
            int num = rand();
            numbers[i] = bformat("%d", num);
            data[i] = bformat("data %d", num);
            BSTree_set(store, numbers[i], data[i]);
        }
    
        for(i = 0; i < 100; i++) {
            bstring value = BSTree_delete(store, numbers[i]);
            mu_assert(value == data[i], "Failed to delete the right number.");
    
            mu_assert(BSTree_delete(store, numbers[i]) == NULL, "Should get nothing.");
    
            for(j = i+1; j < 99 - i; j++) {
                bstring value = BSTree_get(store, numbers[j]);
                mu_assert(value == data[j], "Failed to get the right number.");
            }
    
            bdestroy(value);
            bdestroy(numbers[i]);
        }
    
        BSTree_destroy(store);
    
        return NULL;
    }
    
    char *all_tests()
    {
        mu_suite_start();
    
        mu_run_test(test_create);
        mu_run_test(test_get_set);
        mu_run_test(test_traverse);
        mu_run_test(test_delete);
        mu_run_test(test_destroy);
        mu_run_test(test_fuzzing);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

I'll point you at the test_fuzzing function, which is an interesting technique for testing complex data structures. It is difficult to create a set of keys that cover all the branches in BSTree_node_delete, and chances are I would miss some edge case. A better way is to create a "fuzz" function that does all the operations, but does them in as horrible and random a way as possible. In this case I'm inserting a set of random string keys, then I'm deleting them and trying to get the rest after each delete.

Doing this prevents the situation where you test only what you know to work, which means you'll miss things you don't know. By throwing random junk at your data structures you'll hit things you didn't expect and work out any bugs you have.

# How To Improve It

Do not do any of these yet since in the next exercise I'll be using this unit test to teach you some more performance tuning tricks. You'll come back and do these after you do Exercise 41.

   As usual, you should go through all the defensive programming checks and add assert``s for conditions that shouldn't happen.  For example, you should not be getting ``NULL values for the recursion functions, so assert that.
   The traverse function traverses the tree in order by traversing left, then right, then the current node. You can create traverse functions for reverse order as well.
   It does a full string compare on every node, but I could use the Hashmap hashing functions to speed this up. I could hash the keys, then keep the hash in the BSTreeNode. Then in each of the set up functions I can hash the key ahead of time, and pass it down to the recursive function. Using this hash I can then compare each node much quicker similar to I do in Hashmap.

# Extra Credit

Again, do not do these yet, wait until Exercise 41 when you can use performance tuning features of Valgrind to do them.

   There's an alternative way to do this data structure without using recursion. The Wikipedia page shows alternatives that don't use recursion but do the same thing. Why would this be better or worse?
   Read up on all the different similar trees you can find. There's AVL trees, Red-Black trees, and some non-tree structures like Skip Lists.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















