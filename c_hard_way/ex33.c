Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 33: Linked List Algorithms

I'm going to cover two algorithms you can do on a linked list that involve sorting. I'm going to warn you first that if you need to sort the data, then don't use a linked list. These are horrible for sorting things, and there's much better data structures you can use if that's a requirement. I'm covering these two algorithms because they are slightly difficult to pull off with a linked list and get you thinking about manipulating them efficiently.

In the interest of writing this book, I'm going to put the algorithms in two different files list_algos.h and list_algos.c then write a test in list_algos_test.c. For now just follow my structure, as it does keep things clean, but if you ever work on other libraries remember this isn't a common structure.

In this exercise I'm going to also give you an extra challenge and I want you to try not to cheat. I'm going to give you the unit test first, and I want you to type it in. Then I want you to try and implement the two algorithms based on their descriptions in Wikipedia before seeing if your code is like my code.

# Bubble And Merge Sort

You know what's awesome about the Internet? I can just link you to the Bubble Sort page Merge Sort page and tell you to read that. Man, that saves me a boat load of typing. Now I can tell you how to actually implement each of these using the pseudo-code they have there. Here's how you can tackle an algorithm like this:

   Read the description and look at any visualizations it has.
   Either draw the algorithm on paper using boxes and lines, or actually take a deck of numbered cards (like Poker Cards) and try to do the algorithm manually. This gives you a concrete demonstration of how the algorithm works.
   Create the skeleton functions in your list_algos.c file and make a working list_algos.h file, then setup your test harness.
   Write your first failing test and get everything to compile.
   Go back to the Wikipedia page and copy-paste the pseudo-code (not the C code!) into the guts of the first function you're making.
   Translate this pseudo-code into good C code like I've taught you, using your unit test to make sure it's working.
   Fill out some more tests for edge cases like, empty lists, already sorted lists, etc.
   Repeat for the next algorithm and test.

I just gave you the secret to figuring out most of the algorithms out there, that is until you get to some of the more insane ones. In this case you're just doing the Bubble and Merge Sorts from Wikipedia, but those will be good starters.

# The Unit Test

Here is the unit test you should try to get passing:
    
    #include "minunit.h"
    #include <lcthw/list_algos.h>
    #include <assert.h>
    #include <string.h>
    
    char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
    #define NUM_VALUES 5
    
    List *create_words()
    {
        int i = 0;
        List *words = List_create();
    
        for(i = 0; i < NUM_VALUES; i++) {
            List_push(words, values[i]);
        }
    
        return words;
    }
    
    int is_sorted(List *words)
    {
        LIST_FOREACH(words, first, next, cur) {
            if(cur->next && strcmp(cur->value, cur->next->value) > 0) {
                debug("%s %s", (char *)cur->value, (char *)cur->next->value);
                return 0;
            }
        }
    
        return 1;
    }
    
    char *test_bubble_sort()
    {
        List *words = create_words();
    
        // should work on a list that needs sorting
        int rc = List_bubble_sort(words, (List_compare)strcmp);
        mu_assert(rc == 0, "Bubble sort failed.");
        mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");
    
        // should work on an already sorted list
        rc = List_bubble_sort(words, (List_compare)strcmp);
        mu_assert(rc == 0, "Bubble sort of already sorted failed.");
        mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");
    
        List_destroy(words);
    
        // should work on an empty list
        words = List_create(words);
        rc = List_bubble_sort(words, (List_compare)strcmp);
        mu_assert(rc == 0, "Bubble sort failed on empty list.");
        mu_assert(is_sorted(words), "Words should be sorted if empty.");
    
        List_destroy(words);
    
        return NULL;
    }
    
    char *test_merge_sort()
    {
        List *words = create_words();
    
        // should work on a list that needs sorting
        List *res = List_merge_sort(words, (List_compare)strcmp);
        mu_assert(is_sorted(res), "Words are not sorted after merge sort.");
    
        List *res2 = List_merge_sort(res, (List_compare)strcmp);
        mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
        List_destroy(res2);
        List_destroy(res);
    
        List_destroy(words);
        return NULL;
    }
    
    
    char *all_tests()
    {
        mu_suite_start();
    
        mu_run_test(test_bubble_sort);
        mu_run_test(test_merge_sort);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

I suggest that you start with the bubble sort and get that working, then move on to the merge sort. What I would do is lay out the function prototypes and skeletons that get all three files compiling, but not passing the test. Then just fill in the implementation until it starts working.

# The Implementation

Are you cheating? In future exercises I will do exercises where I just give you a unit test and tell you to implement it, so it'll be good practice for you to not look at this code until you get your own working. Here's the code for the list_algos.c and list_algos.h:
    
    #ifndef lcthw_List_algos_h
    #define lcthw_List_algos_h
    
    #include <lcthw/list.h>
    
    typedef int (*List_compare)(const void *a, const void *b);
    
    int List_bubble_sort(List *list, List_compare cmp);
    
    List *List_merge_sort(List *list, List_compare cmp);
    
    #endif
    
    
    #include <lcthw/list_algos.h>
    #include <lcthw/dbg.h>
    
    inline void ListNode_swap(ListNode *a, ListNode *b)
    {
        void *temp = a->value;
        a->value = b->value;
        b->value = temp;
    }
    
    int List_bubble_sort(List *list, List_compare cmp)
    {
        int sorted = 1;
    
        if(List_count(list) <= 1) {
            return 0;  // already sorted
        }
    
        do {
            sorted = 1;
            LIST_FOREACH(list, first, next, cur) {
                if(cur->next) {
                    if(cmp(cur->value, cur->next->value) > 0) {
                        ListNode_swap(cur, cur->next);
                        sorted = 0;
                    }
                }
            }
        } while(!sorted);
    
        return 0;
    }
    
    inline List *List_merge(List *left, List *right, List_compare cmp)
    {
        List *result = List_create();
        void *val = NULL;
    
        while(List_count(left) > 0 || List_count(right) > 0) {
            if(List_count(left) > 0 && List_count(right) > 0) {
                if(cmp(List_first(left), List_first(right)) <= 0) {
                    val = List_shift(left);
                } else {
                    val = List_shift(right);
                }
    
                List_push(result, val);
            } else if(List_count(left) > 0) {
                val = List_shift(left);
                List_push(result, val);
            } else if(List_count(right) > 0) {
                val = List_shift(right);
                List_push(result, val);
            }
        }
    
        return result;
    }
    
    List *List_merge_sort(List *list, List_compare cmp)
    {
        if(List_count(list) <= 1) {
            return list;
        }
    
        List *left = List_create();
        List *right = List_create();
        int middle = List_count(list) / 2;
    
        LIST_FOREACH(list, first, next, cur) {
            if(middle > 0) {
                List_push(left, cur->value);
            } else {
                List_push(right, cur->value);
            }
    
            middle--;
        }
    
        List *sort_left = List_merge_sort(left, cmp);
        List *sort_right = List_merge_sort(right, cmp);
    
        if(sort_left != left) List_destroy(left);
        if(sort_right != right) List_destroy(right);
    
        return List_merge(sort_left, sort_right, cmp);
    }
    

The bubble sort isn't too bad to figure out, although it is really slow. The merge sort is much more complicated, and honestly I could probably spend a bit more time optimizing this code if I wanted to sacrifice clarity.

There is another way to implement merge sort using a "bottom up" method, but it's a little harder to understand so I didn't do it. As I've already said, sorting algorithms on linked lists are entirely pointless. You could spend all day trying to make this faster and it will still be slower than almost any other sortable data structure. The nature of linked lists is such that you simply don't use them if you need to sort things.

# What You Should See

If everything works then you should get something like this:
    
    $ make clean all
    rm -rf build src/lcthw/list.o src/lcthw/list_algos.o tests/list_algos_tests tests/list_tests
    rm -f tests/tests.log 
    find . -name "*.gc*" -exec rm {} \;
    rm -rf `find . -name "*.dSYM" -print`
    cc -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG  -fPIC   -c -o src/lcthw/list.o src/lcthw/list.c
    cc -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG  -fPIC   -c -o src/lcthw/list_algos.o src/lcthw/list_algos.c
    ar rcs build/liblcthw.a src/lcthw/list.o src/lcthw/list_algos.o
    ranlib build/liblcthw.a
    cc -shared -o build/liblcthw.so src/lcthw/list.o src/lcthw/list_algos.o
    cc -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG  build/liblcthw.a    tests/list_algos_tests.c   -o tests/list_algos_tests
    cc -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG  build/liblcthw.a    tests/list_tests.c   -o tests/list_tests
    sh ./tests/runtests.sh
    Running unit tests:
    ----
    RUNNING: ./tests/list_algos_tests
    ALL TESTS PASSED
    Tests run: 2
    tests/list_algos_tests PASS
    ----
    RUNNING: ./tests/list_tests
    ALL TESTS PASSED
    Tests run: 6
    tests/list_tests PASS
    $
    

After this exercise I'm not going to show you this output unless it's necessary to show you how it works. From now on you should know that I ran the tests and they all passed and everything compiled.

# How To Improve It

Going back to the description of the algorithms, there's several ways to improve these implementations, and there's a few obvious ones:

   The merge sort does a crazy amount of copying and creating lists, find ways to reduce this.
   The bubble sort Wikipedia description mentions a few optimizations, implement them.
   Can you use the List_split and List_join (if you implemented them) to improve merge sort?
   Go through all the defensive programming checks and improve the robustness of this implementation, protecting against bad NULL pointers, and create an optional debug level invariant that does what is_sorted does after a sort.

# Extra Credit

   Create a unit test that compares the performance of the two algorithms. You'll want to look at man 3 time for a basic timer function, and you'll want to run enough iterations to at least have a few seconds of samples.
   Play with the amount of data in the lists that need to be sorted and see if that changes your timing.
   Find a way to simulate filling different sized random lists and measuring how long they take, then graph it and see how it compares to the description of the algorithm.
   Try to explain why sorting linked lists is a really bad idea.
   Implement a List_insert_sorted that will take a given value, and using the List_compare, insert the element at the right position so that the list is always sorted. How does using this method compare to sorting a list after you've built it?
   Try implementing the "bottom up" merge sort on the wikipedia page. The code there is already C so it should be easy to recreate, but try to understand how it's working compared to the slower one I have here.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 



















