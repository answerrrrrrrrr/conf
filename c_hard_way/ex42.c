Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 42: Stacks and Queues

At this point in the book you know most of the data structures that are used to build all the other data structures. If you have some kind of List, DArray, Hashmap, and Tree then you can build most anything else that's out there. Everything else you run into either uses these or is some variant on these. If it's not then it's most likely an exotic data structure that you probably will not need.

Stacks and Queues are very simple data structures that are really variants of the List data structure. All they are is using a List with a "discipline" or convention that says you'll always place elements on one end of the List. For a Stack, you always push and pop. For a Queue, you always shift to the front, but pop from the end.

I can implement both data structures using nothing but the CPP and two header files. My header files are 21 lines long and do all the Stack and Queue operations without any fancy defines.

To see if you've been paying attention, I'm going to show you the unit tests, and then you have to implement the header files needed to make them work. To pass this exercise you can't create any stack.c or queue.c implementation files. Use only the stack.h and queue.h files to make the tests runs.
    
    #include "minunit.h"
    #include <lcthw/stack.h>
    #include <assert.h>
    
    static Stack *stack = NULL;
    char *tests[] = {"test1 data", "test2 data", "test3 data"};
    #define NUM_TESTS 3
    
    
    char *test_create()
    {
        stack = Stack_create();
        mu_assert(stack != NULL, "Failed to create stack.");
    
        return NULL;
    }
    
    char *test_destroy()
    {
        mu_assert(stack != NULL, "Failed to make stack #2");
        Stack_destroy(stack);
    
        return NULL;
    }
    
    char *test_push_pop()
    {
        int i = 0;
        for(i = 0; i < NUM_TESTS; i++) {
            Stack_push(stack, tests[i]);
            mu_assert(Stack_peek(stack) == tests[i], "Wrong next value.");
        }
    
        mu_assert(Stack_count(stack) == NUM_TESTS, "Wrong count on push.");
    
        STACK_FOREACH(stack, cur) {
            debug("VAL: %s", (char *)cur->value);
        }
    
        for(i = NUM_TESTS - 1; i >= 0; i--) {
            char *val = Stack_pop(stack);
            mu_assert(val == tests[i], "Wrong value on pop.");
        }
    
        mu_assert(Stack_count(stack) == 0, "Wrong count after pop.");
    
        return NULL;
    }
    
    char *all_tests() {
        mu_suite_start();
    
        mu_run_test(test_create);
        mu_run_test(test_push_pop);
        mu_run_test(test_destroy);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

Then the queue_tests.c is almost the same just using Queue:
    
    #include "minunit.h"
    #include <lcthw/queue.h>
    #include <assert.h>
    
    static Queue *queue = NULL;
    char *tests[] = {"test1 data", "test2 data", "test3 data"};
    #define NUM_TESTS 3
    
    
    char *test_create()
    {
        queue = Queue_create();
        mu_assert(queue != NULL, "Failed to create queue.");
    
        return NULL;
    }
    
    char *test_destroy()
    {
        mu_assert(queue != NULL, "Failed to make queue #2");
        Queue_destroy(queue);
    
        return NULL;
    }
    
    char *test_send_recv()
    {
        int i = 0;
        for(i = 0; i < NUM_TESTS; i++) {
            Queue_send(queue, tests[i]);
            mu_assert(Queue_peek(queue) == tests[0], "Wrong next value.");
        }
    
        mu_assert(Queue_count(queue) == NUM_TESTS, "Wrong count on send.");
    
        QUEUE_FOREACH(queue, cur) {
            debug("VAL: %s", (char *)cur->value);
        }
    
        for(i = 0; i < NUM_TESTS; i++) {
            char *val = Queue_recv(queue);
            mu_assert(val == tests[i], "Wrong value on recv.");
        }
    
        mu_assert(Queue_count(queue) == 0, "Wrong count after recv.");
    
        return NULL;
    }
    
    char *all_tests() {
        mu_suite_start();
    
        mu_run_test(test_create);
        mu_run_test(test_send_recv);
        mu_run_test(test_destroy);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

# What You Should See

Your unit test should run without you changing the tests, and it should pass valgrind with no memory errors. Here's what it looks like if I run stack_tests directly:
    
    $ ./tests/stack_tests
    DEBUG tests/stack_tests.c:60: ----- RUNNING: ./tests/stack_tests
    ----
    RUNNING: ./tests/stack_tests
    DEBUG tests/stack_tests.c:53: 
    ----- test_create
    DEBUG tests/stack_tests.c:54: 
    ----- test_push_pop
    DEBUG tests/stack_tests.c:37: VAL: test3 data
    DEBUG tests/stack_tests.c:37: VAL: test2 data
    DEBUG tests/stack_tests.c:37: VAL: test1 data
    DEBUG tests/stack_tests.c:55: 
    ----- test_destroy
    ALL TESTS PASSED
    Tests run: 3
    $
    

The queue_test is basically the same kind of output so I shouldn't have to show it to you at this stage.

# How To Improve It

The only real improvement you could make to this is to switch from using a List to using a DArray. The Queue data structure is more difficult to do with a DArray because it works at both ends of the list of nodes.

A disadvantage of doing this entirely in a header file is that you can't easily performance tune it. Mostly what you're doing with this technique is establishing a "protocol" for how to use a List in a certain style. When performance tuning, if you make List fast then these two should improve as well.

# Extra Credit

   Implement Stack using DArray instead of List without changing the unit test. That means you'll have to create your own STACK_FOREACH.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















