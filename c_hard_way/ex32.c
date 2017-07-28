Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 32: Double Linked Lists

The purpose of this book is to teach you how your computer really works, and included in that is how various data structures and algorithms function. Computers by themselves don't do a lot of useful processing. To make them do useful things you need to structure the data and then organize processing on these structures. Other programming languages either include libraries that implement all of these structures, or they have direct syntax for them. C makes you implement all the data structures you need yourself, which makes it the perfect language to learn how they actually work.

My goal in teaching you these data structures and these algorithms is to help you do three things:

   Understand what is really going on in Python, Ruby, or JavaScript code like: data = {"name": "Zed"}
   Get even better at C code by applying what you know to a set of solved problems using the data structures.
   Learn a core set of data structures and algorithms so that you are better informed about what ones work best in certain situations.

# What Are Data Structures

The name "data structure" is self-explanatory. It is an organization of data that fits a certain model. Maybe the model is designed to allow processing the data in a new way. Maybe it's just organized to store it on disk efficiently. In this book I'll follow a simple pattern for making data structures that works reliably:

   Define a struct for the main "outer structure".
   Define a struct for the contents, usually nodes with links between them.
   Create functions that operate on these two.

There's other styles of data structures in C, but this pattern works well and is consistent for most data structures you'll make.

# Making The Library

For the rest of this book you'll be creating a library that you can use when you're done with this book. This library will have the following elements:

   Header (.h) files for each data structure.
   Implementation (.c) files for the algorithms.
   Unit tests that test all of them to make sure they keep working.
   Documentation we'll autogenerate from the header files.

You already have the c-skeleton so use it to create a liblcthw project:
    
    $ cp -r c-skeleton liblcthw
    $ cd liblcthw/
    $ ls
    LICENSE             Makefile        README.md       bin             build           src             tests
    $ vim Makefile
    $ ls src/
    dbg.h               libex29.c       libex29.o
    $ mkdir src/lcthw
    $ mv src/dbg.h src/lcthw
    $ vim tests/minunit.h
    $ rm src/libex29.* tests/libex29*
    $ make clean
    rm -rf build  tests/libex29_tests
    rm -f tests/tests.log 
    find . -name "*.gc*" -exec rm {} \;
    rm -rf `find . -name "*.dSYM" -print`
    $ ls tests/
    minunit.h  runtests.sh
    $
    

In this session I'm doing the following:

   Copy the c-skeleton over.
   Edit the Makefile to change libYOUR_LIBRARY.a to liblcthw.a as the new TARGET.
   Make the src/lcthw directory where we'll put our code.
   Move the src/dbg.h into this new directory.
   Edit tests/minunit.h so that it uses #include <lcthw/dbg.h> as the include.
   Get rid of the source and test files we don't need for libex29.*.
   Clean up everything that's left over.

With that you're ready to start building the library, and the first data structure I'll build is the Double Linked List.

# Double Linked Lists

The first data structure we'll add to liblcthw is a double linked list. This is the simplest data structure you can make, and it has useful properties for certain operations. A linked list works by nodes having pointers to their next or previous element. A "double linked list" contains pointers to both, while a "single linked list" only points at the next element.

Because each node has pointers to the next and previous, and because you keep track of the first and last element of the list, you can do some operations very quickly. Anything that involves inserting or deleting an element will be very fast. They are also easy to implement by most people.

The main disadvantage of a linked list is that traversing it involves processing every single pointer along the way. This means that searching, most sorting, or iterating over the elements will be slow. It also means that you can't really jump to random parts of the list. If you had an array of elements you could just index right into the middle of the list, but a linked list uses a stream of pointers. That means if you want the 10th element, you have to go through elements 1-9.

## Definition

As I said in the introduction to this exercise, the process to follow is to first write a header file with the right C struct statements in it.
    
    #ifndef lcthw_List_h
    #define lcthw_List_h
    
    #include <stdlib.h>
    
    struct ListNode;
    
    typedef struct ListNode {
        struct ListNode *next;
        struct ListNode *prev;
        void *value;
    } ListNode;
    
    typedef struct List {
        int count;
        ListNode *first;
        ListNode *last;
    } List;
    
    List *List_create();
    void List_destroy(List *list);
    void List_clear(List *list);
    void List_clear_destroy(List *list);
    
    #define List_count(A) ((A)->count)
    #define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
    #define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)
    
    void List_push(List *list, void *value);
    void *List_pop(List *list);
    
    void List_unshift(List *list, void *value);
    void *List_shift(List *list);
    
    void *List_remove(List *list, ListNode *node);
    
    #define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
        ListNode *V = NULL;\
        for(V = _node = L->S; _node != NULL; V = _node = _node->M)
    
    #endif
    

The first thing I do is create two structs for the ListNode and the List that will contain those nodes. This creates the data structure I'll use in the functions and macros I define after that. If you read through these functions they seem rather simple. I'll be explaining them when I cover the implementation, but hopefully you can guess what they do.

How the data structure works is each ListNode has three components:

   A value, which is a pointer to anything and stores the thing we want to put in the list.
   A ListNode *next pointer which points at another ListNode that holds the next element in the list.
   A ListNode *prev that holds the previous element. Complex right? Calling the previous thing "previous". I could have used "anterior" and "posterior" but only a jerk would do that.

The List struct is then nothing more than a container for these ListNode structs that have been linked together in a chain. It keeps track of the count, first and last element of the list.

Finally, take a look at src/lcthw/list.h:37 where I define the LIST_FOREACH macro. This is a common idiom where you make a macro that generates iteration code so people can't mess it up. Getting this kind of processing right can be difficult with data structures, so writing macros helps people out. You'll see how I use this when I talk about the implementation.

## Implementation

Once you understand that, you mostly understand how a double linked list works. It is nothing more than nodes with two pointers to the next and previous element of the list. You can then write the src/lcthw/list.c code to see how each operation is implemented.
    
    #include <lcthw/list.h>
    #include <lcthw/dbg.h>
    
    List *List_create()
    {
        return calloc(1, sizeof(List));
    }
    
    void List_destroy(List *list)
    {
        LIST_FOREACH(list, first, next, cur) {
            if(cur->prev) {
                free(cur->prev);
            }
        }
    
        free(list->last);
        free(list);
    }
    
    
    void List_clear(List *list)
    {
        LIST_FOREACH(list, first, next, cur) {
            free(cur->value);
        }
    }
    
    
    void List_clear_destroy(List *list)
    {
        List_clear(list);
        List_destroy(list);
    }
    
    
    void List_push(List *list, void *value)
    {
        ListNode *node = calloc(1, sizeof(ListNode));
        check_mem(node);
    
        node->value = value;
    
        if(list->last == NULL) {
            list->first = node;
            list->last = node;
        } else {
            list->last->next = node;
            node->prev = list->last;
            list->last = node;
        }
    
        list->count++;
    
    error:
        return;
    }
    
    void *List_pop(List *list)
    {
        ListNode *node = list->last;
        return node != NULL ? List_remove(list, node) : NULL;
    }
    
    void List_unshift(List *list, void *value)
    {
        ListNode *node = calloc(1, sizeof(ListNode));
        check_mem(node);
    
        node->value = value;
    
        if(list->first == NULL) {
            list->first = node;
            list->last = node;
        } else {
            node->next = list->first;
            list->first->prev = node;
            list->first = node;
        }
    
        list->count++;
    
    error:
        return;
    }
    
    void *List_shift(List *list)
    {
        ListNode *node = list->first;
        return node != NULL ? List_remove(list, node) : NULL;
    }
    
    void *List_remove(List *list, ListNode *node)
    {
        void *result = NULL;
    
        check(list->first && list->last, "List is empty.");
        check(node, "node can't be NULL");
    
        if(node == list->first && node == list->last) {
            list->first = NULL;
            list->last = NULL;
        } else if(node == list->first) {
            list->first = node->next;
            check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
            list->first->prev = NULL;
        } else if (node == list->last) {
            list->last = node->prev;
            check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
            list->last->next = NULL;
        } else {
            ListNode *after = node->next;
            ListNode *before = node->prev;
            after->prev = before;
            before->next = after;
        }
    
        list->count--;
        result = node->value;
        free(node);
    
    error:
        return result;
    }
    

I then implement all of the operations on a double linked list that can't be done with simple macros. Rather than cover every tiny little line of this file, I'm going to give high-level overview of every operation in both the list.h and list.c file, then leave you to read the code.

list.h:List_count
    Returns the number of elements in the list, which is maintained as elements are added and removed.
list.h:List_first
    Returns the first element of the list, but does not remove it.
list.h:List_last
    Returns the last element of the list, but does not remove it.
list.h:LIST_FOREACH
    Iterates over the elements in the list.
list.c:List_create
    Simply creates the main List struct.
list.c:List_destroy
    Destroys a List and any elements it might have.
list.c:List_clear
    Convenience function for freeing the values in each node, not the nodes.
list.c:List_clear_destroy
    Clears and destroys a list. It's not very efficient since it loops through them twice.
list.c:List_push
    The first operation that demonstrates the advantage of a linked list. It adds a new element to the end of the list, and because that's just a couple of pointer assignments, does it very fast.
list.c:List_pop
    The inverse of List_push, this takes the last element off and returns it.
list.c:List_unshift
    The other thing you can easily do to a linked list is add elements to the front of the list very fast. In this case I call that List_unshift for lack of a better term.
list.c:List_shift
    Just like List_pop, this removes the first element and returns it.
list.c:List_remove
    This is actually doing all of the removal when you do List_pop or List_shift. Something that seems to always be difficult in data structures is removing things, and this function is no different. It has to handle quite a few conditions depending on if the element being removed is at the front; the end; both front and end; or middle.

Most of these functions are nothing special, and you should be able to easily digest this and understand it from just the code. You should definitely focus on how the LIST_FOREACH macro is used in List_destroy so you can understand how much it simplifies this common operation.

# Tests

After you have those compiling it's time to create the test that makes sure they operate correctly.
    
    #include "minunit.h"
    #include <lcthw/list.h>
    #include <assert.h>
    
    static List *list = NULL;
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";
    
    
    char *test_create()
    {
        list = List_create();
        mu_assert(list != NULL, "Failed to create list.");
    
        return NULL;
    }
    
    
    char *test_destroy()
    {
        List_clear_destroy(list);
    
        return NULL;
    
    }
    
    
    char *test_push_pop()
    {
        List_push(list, test1);
        mu_assert(List_last(list) == test1, "Wrong last value.");
    
        List_push(list, test2);
        mu_assert(List_last(list) == test2, "Wrong last value");
    
        List_push(list, test3);
        mu_assert(List_last(list) == test3, "Wrong last value.");
        mu_assert(List_count(list) == 3, "Wrong count on push.");
    
        char *val = List_pop(list);
        mu_assert(val == test3, "Wrong value on pop.");
    
        val = List_pop(list);
        mu_assert(val == test2, "Wrong value on pop.");
    
        val = List_pop(list);
        mu_assert(val == test1, "Wrong value on pop.");
        mu_assert(List_count(list) == 0, "Wrong count after pop.");
    
        return NULL;
    }
    
    char *test_unshift()
    {
        List_unshift(list, test1);
        mu_assert(List_first(list) == test1, "Wrong first value.");
    
        List_unshift(list, test2);
        mu_assert(List_first(list) == test2, "Wrong first value");
    
        List_unshift(list, test3);
        mu_assert(List_first(list) == test3, "Wrong last value.");
        mu_assert(List_count(list) == 3, "Wrong count on unshift.");
    
        return NULL;
    }
    
    char *test_remove()
    {
        // we only need to test the middle remove case since push/shift 
        // already tests the other cases
    
        char *val = List_remove(list, list->first->next);
        mu_assert(val == test2, "Wrong removed element.");
        mu_assert(List_count(list) == 2, "Wrong count after remove.");
        mu_assert(List_first(list) == test3, "Wrong first after remove.");
        mu_assert(List_last(list) == test1, "Wrong last after remove.");
    
        return NULL;
    }
    
    
    char *test_shift()
    {
        mu_assert(List_count(list) != 0, "Wrong count before shift.");
    
        char *val = List_shift(list);
        mu_assert(val == test3, "Wrong value on shift.");
    
        val = List_shift(list);
        mu_assert(val == test1, "Wrong value on shift.");
        mu_assert(List_count(list) == 0, "Wrong count after shift.");
    
        return NULL;
    }
    
    
    
    char *all_tests() {
        mu_suite_start();
    
        mu_run_test(test_create);
        mu_run_test(test_push_pop);
        mu_run_test(test_unshift);
        mu_run_test(test_remove);
        mu_run_test(test_shift);
        mu_run_test(test_destroy);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

This test simply goes through every operation and makes sure it works. I use a simplification in the test where I create just one List *list for the whole program, then have the tests work on it. This saves the trouble of building a List for every test, but it could mean that some tests only pass because of how the previous test ran. In this case I try to make each test keep the list clear or actually use the previous test's results.

# What You Should See

If you did everything right, then when you do a build and run the unit tests it should look like this:
    
    $ make
    cc -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG  -fPIC   -c -o src/lcthw/list.o src/lcthw/list.c
    ar rcs build/liblcthw.a src/lcthw/list.o
    ranlib build/liblcthw.a
    cc -shared -o build/liblcthw.so src/lcthw/list.o
    cc -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG  build/liblcthw.a    tests/list_tests.c   -o tests/list_tests
    sh ./tests/runtests.sh
    Running unit tests:
    ----
    RUNNING: ./tests/list_tests
    ALL TESTS PASSED
    Tests run: 6
    tests/list_tests PASS
    $
    

Make sure 6 tests ran, that it builds without warnings or errors, and that it's making the build/liblcthw.a and build/liblcthw.so files.

# How To Improve It

Instead of breaking this, I'm going to tell you how to improve the code:

   You can make List_clear_destroy more efficient by using LIST_FOREACH and doing both free calls inside one loop.
   You can add asserts for preconditions that it isn't given a NULL value for the List *list parameters.
   You can add invariants that check the list's contents are always correct, such as count is never < 0, and if count > 0 then first isn't NULL.
   You can add documentation to the header file in the form of comments before each struct, function, and macro that describes what it does.

These amount to going through the defensive programming practices I talked about and "hardening" this code against flaws or improving usability. Go ahead and do these things, then find as many other ways to improve the code.

# Extra Credit

   Research double vs. single linked lists and when one is preferred over the other.
   Research the limitations of a double linked list. For example, while they are efficient for inserting and deleting elements, they are very slow for iterating over them all.
   What operations are missing that you can imagine needing? Some examples are copying, joining, splitting. Implement these operations and write the unit tests for them.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















