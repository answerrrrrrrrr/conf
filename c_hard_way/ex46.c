Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 46: Ternary Search Tree

The final data structure I'll show you is call the TSTree and it's similar to the BSTree except it has three branches low, equal, and high. It's primarily used to just like BSTree and Hashmap to store key/value data, but it is keyed off of the individual characters in the keys. This gives the TSTree some abilities that neither BSTree or Hashmap have.

How a TSTree works is every key is a string, and it's inserted by walking and building a tree based on the equality of the characters in the string. Start at the root, look at the character for that node, and if lower, equal to, or higher than that then go in that direction. You can see this in the header file:
    
    #ifndef _lcthw_TSTree_h
    #define _lctwh_TSTree_h
    
    #include <stdlib.h>
    #include <lcthw/darray.h>
    
    typedef struct TSTree {
        char splitchar;
        struct TSTree *low;
        struct TSTree *equal;
        struct TSTree *high;
        void *value;
    } TSTree;
    
    void *TSTree_search(TSTree *root, const char *key, size_t len);
    
    void *TSTree_search_prefix(TSTree *root, const char *key, size_t len);
    
    typedef void (*TSTree_traverse_cb)(void *value, void *data);
    
    TSTree *TSTree_insert(TSTree *node, const char *key, size_t len, void *value);
    
    void TSTree_traverse(TSTree *node, TSTree_traverse_cb cb, void *data);
    
    void TSTree_destroy(TSTree *root);
    
    #endif
    

The TSTree has the following elements:

splitchar
    The character at this point in the tree.
low
    The branch that is lower than splitchar.
equal
    The branch that is equal to splitchar.
high
    The branch that is higher than splitchar.
value
    The value set for a string at that point with that splitchar.

You can see this implementation has the following operations:

search
    Typical "find a value for this key" operation.
search_prefix
    Finds the first value that has this as a prefix of its key. This is the an operation that you can't easily do in a BSTree or Hashmap.
insert
    Breaks the key down by each character and inserts it into the tree.
traverse
    Walks the tree allowing you to collect or analyze all the keys and values it contains.

The only thing missing is a TSTree_delete, and that's because it is a horribly expensive operation, even more than BSTree_delete was. When I use TSTree structures I treat them as constant data that I plan on traversing many times and not removing anything from them. They are very fast for this, but are not good if you need to insert and delete quickly. For that I use Hashmap since it beats both BSTree and TSTree.

The implementation for the TSTree is actually simple, but it might be hard to follow at first. I'll break it down after you enter it in:
    
    #include <stdlib.h>
    #include <stdio.h>
    #include <assert.h>
    #include <lcthw/dbg.h>
    #include <lcthw/tstree.h>
    
    static inline TSTree *TSTree_insert_base(TSTree *root, TSTree *node,
            const char *key, size_t len, void *value)
    {
        if(node == NULL) {
            node = (TSTree *) calloc(1, sizeof(TSTree));
    
            if(root == NULL) {
                root = node;
            }
    
            node->splitchar = *key;
        }
    
        if(*key < node->splitchar) {
            node->low = TSTree_insert_base(root, node->low, key, len, value);
        } else if(*key == node->splitchar) {
            if(len > 1) {
                node->equal = TSTree_insert_base(root, node->equal, key+1, len - 1, value);
            } else {
                assert(node->value == NULL && "Duplicate insert into tst.");
                node->value = value;
            }
        } else {
            node->high = TSTree_insert_base(root, node->high, key, len, value);
        }
    
        return node;
    }
    
    TSTree *TSTree_insert(TSTree *node, const char *key, size_t len, void *value)
    {
        return TSTree_insert_base(node, node, key, len, value);
    }
    
    void *TSTree_search(TSTree *root, const char *key, size_t len)
    {
        TSTree *node = root;
        size_t i = 0;
    
        while(i < len && node) {
            if(key[i] < node->splitchar) {
                node = node->low;
            } else if(key[i] == node->splitchar) {
                i++;
                if(i < len) node = node->equal;
            } else {
                node = node->high;
            }
        }
    
        if(node) {
            return node->value;
        } else {
            return NULL;
        }
    }
    
    void *TSTree_search_prefix(TSTree *root, const char *key, size_t len)
    {
        if(len == 0) return NULL;
    
        TSTree *node = root;
        TSTree *last = NULL;
        size_t i = 0;
    
        while(i < len && node) {
            if(key[i] < node->splitchar) {
                node = node->low;
            } else if(key[i] == node->splitchar) {
                i++;
                if(i < len) {
                    if(node->value) last = node;
                    node = node->equal;
                }
            } else {
                node = node->high;
            }
        }
    
        node = node ? node : last;
    
        // traverse until we find the first value in the equal chain
        // this is then the first node with this prefix
        while(node && !node->value) {
            node = node->equal;
        }
    
        return node ? node->value : NULL;
    }
    
    void TSTree_traverse(TSTree *node, TSTree_traverse_cb cb, void *data)
    {
        if(!node) return;
    
        if(node->low) TSTree_traverse(node->low, cb, data);
    
        if(node->equal) {
            TSTree_traverse(node->equal, cb, data);
        }
    
        if(node->high) TSTree_traverse(node->high, cb, data);
    
        if(node->value) cb(node->value, data);
    }
    
    void TSTree_destroy(TSTree *node)
    {
        if(node == NULL) return;
    
        if(node->low) TSTree_destroy(node->low);
    
        if(node->equal) {
            TSTree_destroy(node->equal);
        }
    
        if(node->high) TSTree_destroy(node->high);
    
        free(node);
    }
    

For TSTree_insert I'm using the same pattern for recursive structures where I have a small function that calls the real recursive function. I'm not doing any additional check here but you should add the usual defensive programming to it. One thing to keep in mind is it is using a slightly different design where you don't have a separate TSTree_create function, and instead if you pass it a NULL for the node then it will create it, and returns the final value.

That means I need to break down TSTree_insert_base for you to understand the insert operation:

tstree.c:10-18
    As I mentioned, if I'm given a NULL then I need to make this node and assign the *key (current char) to it. This is used to build the tree as we insert keys.
tstree.c:20-21
    If the *key < this then recurse, but go to the low branch.
tstree.c:22
    This splitchar is equal, so I want to go to deal with equality. This will happen if we just created this node, so we'll be building the tree at this point.
tstree.c:23-24
    There's still characters to handle, so recurse down the equal branch, but go to the next *key char.
tstree.c:26-27
    This is the last char, so I set the value and that's it. I have an assert here in case of a duplicate.
tstree.c:29-30
    The last condition is that this *key is greater than splitchar so I need to recurse down the high branch.

The key to some of the properties of this data structure is the fact that I'm only incrementing the character I analyze when a splitchar is equal. The other two conditions I just walk the tree until I hit an equal character to recurse into next. What this does is it makes it very fast to not find a key. I can get a bad key, and simply walk a few high and low nodes until I hit a dead end to know that this key doesn't exist. I don't need to process every character of the key, or every node of the tree.

Once you understand that then move onto analyzing how TSTree_search works:

tstree.c:46
    I don't need to process the tree recursively in the TSTree, I can just use a while loop and a node for where I am currently.
tstree.c:47-48
    If the current char is less than the node splitchar, then go low.
tstree.c:49-51
    If it's equal, then increment i and go equal as long as it's not the last character. That's why the if(i < len) is there, so that I don't go too far past the final value.
tstree.c:52-53
    Otherwise I go high since the char is greater.
tstree.c:57-61
    If after the loop I have a node, then return its value, otherwise return NULL.

This isn't too difficult to understand, and you can then see that it's almost exacty the same algorithm for the TSTree_search_prefix function. The only difference is I'm trying to not find an exact match, but the longest prefix I can. To do that I keep track of the last node that was equal, and then after the search loop, walk that node until I find a value.

Looking at TSTree_search_prefix you can start to see the second advantage a TSTree has over the BSTree and Hashmap for finding strings. Given any key of X length, you can find any key in X moves. You can also find the first prefix in X moves, plus N more depending on how big the matching key is. If the biggest key in the tree is 10 characters long, then you can find any prefix in that key in 10 moves. More importantly, you can do all of this by only comparing each character of the key once.

In comparison, to do the same with a BSTree you would have to check the prefixes of each character in every possibly matching node in the BSTree against the characters in the prefix. It's the same for finding keys, or seeing if a key doesn't exist. You have to compare each character against most of the characters in the BSTree to find or not find a match.

A Hashamp is even worse for finding prefixes since you can't hash just the prefix. You basically can't do this efficiently in a Hashmap unless the data is something you can parse like a URL. Even then that usually requires whole trees of Hashmaps.

The last two functions should be easy for you to analyze as they are the typical traversing and destroying operations you've seen already for other data structures.

Finally, I have a simple unit test for the whole thing to make sure it works right:
    
    #include "minunit.h"
    #include <lcthw/tstree.h>
    #include <string.h>
    #include <assert.h>
    #include <lcthw/bstrlib.h>
    
    
    TSTree *node = NULL;
    char *valueA = "VALUEA";
    char *valueB = "VALUEB";
    char *value2 = "VALUE2";
    char *value4 = "VALUE4";
    char *reverse = "VALUER";
    int traverse_count = 0;
    
    struct tagbstring test1 = bsStatic("TEST");
    struct tagbstring test2 = bsStatic("TEST2");
    struct tagbstring test3 = bsStatic("TSET");
    struct tagbstring test4 = bsStatic("T");
    
    char *test_insert()
    {
        node = TSTree_insert(node, bdata(&test1), blength(&test1), valueA);
        mu_assert(node != NULL, "Failed to insert into tst.");
    
        node = TSTree_insert(node, bdata(&test2), blength(&test2), value2);
        mu_assert(node != NULL, "Failed to insert into tst with second name.");
    
        node = TSTree_insert(node, bdata(&test3), blength(&test3), reverse);
        mu_assert(node != NULL, "Failed to insert into tst with reverse name.");
    
        node = TSTree_insert(node, bdata(&test4), blength(&test4), value4);
        mu_assert(node != NULL, "Failed to insert into tst with second name.");
    
        return NULL;
    }
    
    char *test_search_exact()
    {
        // tst returns the last one inserted
        void *res = TSTree_search(node, bdata(&test1), blength(&test1));
        mu_assert(res == valueA, "Got the wrong value back, should get A not B.");
    
        // tst does not find if not exact
        res = TSTree_search(node, "TESTNO", strlen("TESTNO"));
        mu_assert(res == NULL, "Should not find anything.");
    
        return NULL;
    }
    
    char *test_search_prefix()
    {
        void *res = TSTree_search_prefix(node, bdata(&test1), blength(&test1));
        debug("result: %p, expected: %p", res, valueA);
        mu_assert(res == valueA, "Got wrong valueA by prefix.");
    
        res = TSTree_search_prefix(node, bdata(&test1), 1);
        debug("result: %p, expected: %p", res, valueA);
        mu_assert(res == value4, "Got wrong value4 for prefix of 1.");
    
        res = TSTree_search_prefix(node, "TE", strlen("TE"));
        mu_assert(res != NULL, "Should find for short prefix.");
    
        res = TSTree_search_prefix(node, "TE--", strlen("TE--"));
        mu_assert(res != NULL, "Should find for partial prefix.");
    
    
        return NULL;
    }
    
    void TSTree_traverse_test_cb(void *value, void *data)
    {
        assert(value != NULL && "Should not get NULL value.");
        assert(data == valueA && "Expecting valueA as the data.");
        traverse_count++;
    }
    
    char *test_traverse()
    {
        traverse_count = 0;
        TSTree_traverse(node, TSTree_traverse_test_cb, valueA);
        debug("traverse count is: %d", traverse_count);
        mu_assert(traverse_count == 4, "Didn't find 4 keys.");
    
        return NULL;
    }
    
    char *test_destroy()
    {
        TSTree_destroy(node);
    
        return NULL;
    }
    
    char * all_tests() {
        mu_suite_start();
    
        mu_run_test(test_insert);
        mu_run_test(test_search_exact);
        mu_run_test(test_search_prefix);
        mu_run_test(test_traverse);
        mu_run_test(test_destroy);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

# Advantages And Disadvantages

There's other interesting practical things you can do with a TSTree:

   In addition to finding prefixes, you can reverse all the keys you insert, and then find by suffix. I use this to lookup host names, since I want to find *.learncodethehardway.com so if I go backwards I can match them quickly.
   You can do "approximate" matching, where you gather all the nodes that have most of the same characters as the key, or using other algorithms for what's a close match.
   You can find all the keys that have a part in the middle.

I've already talked about some of the things TSTrees can do, but they aren't the best data structure all the time. The disadvantages of the TSTree are:

   As I mentioned, deleting from them is murder. They are better for data that needs to be looked up fast and you rarely remove from. If you need to delete then simply disable the value and then periodically rebuild the tree when it gets too big.
   It uses a ton of memory compared to BSTree and Hashmaps for the same key space. Think about it, it's using a full node for each character in every key. It might do better for smaller keys, but if you put a lot in a TSTree it will get huge.
   They also do not work well with large keys, but "large" is subjective so as usual test first. If you're trying to store 10k character sized keys then use a Hashmap.

# How To Improve It

As usual, go through and improve this by adding the defensive preconditions, asserts, and checks to each function. There's some other possible improvements, but you don't necessarily have to implement all of these:

   You could allow duplicates by using a DArray instead of the value.
   As I mentioned deleting is hard, but you could simulate it by setting the values to NULL so they are effectively gone.
   There are no ways to collect all the possible matching values. I'll have you implement that in an extra credit.
   There are other algorithms that are more complex but have slightly better properties. Take a look at Suffix Array, Suffix Tree, and Radix Tree structures.

# Extra Credit

   Implement a TSTree_collect that returns a DArray containing all the keys that match the given prefix.
   Implement TSTree_search_suffix and a TSTree_insert_suffix so you can do suffix searches and inserts.
   Use valgrind to see how much memory this structure uses to store data compared to the BSTree and Hashmap.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















