Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 30: Automated Testing

Automated testing is used frequently in other languages like Python and Ruby, but rarely used in C. Part of the reason comes from the difficulty of automatically loading and testing pieces of C code. In this chapter we'll create a very small little testing "framework" and get your skeleton directory building an example test case.

The frameworks I'm going to use, and which you'll include in your c-skeleton skeleton is called "minunit" which started with code from a tiny snippet of code by Jera Design. I then evolved it further, to be this:
    
    #undef NDEBUG
    #ifndef _minunit_h
    #define _minunit_h
    
    #include <stdio.h>
    #include <dbg.h>
    #include <stdlib.h>
    
    #define mu_suite_start() char *message = NULL
    
    #define mu_assert(test, message) if (!(test)) { log_err(message); return message; }
    #define mu_run_test(test) debug("\n-----%s", " " #test); \
        message = test(); tests_run++; if (message) return message;
    
    #define RUN_TESTS(name) int main(int argc, char *argv[]) {\
        argc = 1; \
        debug("----- RUNNING: %s", argv[0]);\
            printf("----\nRUNNING: %s\n", argv[0]);\
            char *result = name();\
            if (result != 0) {\
                printf("FAILED: %s\n", result);\
            }\
            else {\
                printf("ALL TESTS PASSED\n");\
            }\
        printf("Tests run: %d\n", tests_run);\
            exit(result != 0);\
    }
    
    
    int tests_run;
    
    #endif
    

There's mostly nothing left of the original, as now I'm using the dbg.h macros and I've created a large macro at the end for the boilerplate test runner. Even with this tiny amount of code we'll create a fully functioning unit test system you can use in your C code once it's combined with a shell script to run the tests.

# Wiring Up The Test Framework

To continue this exercise, you should have your src/libex29.c working and that you completed the Exercise 29 extra credit where you got the ex29.c loader program to properly run. In Exercise 29 I had an extra credit to make it work like a unit test, but I'm going to start over and show you how to do that with minunit.h.

The first thing to do is create a simple empty unit test name tests/libex29_tests.c with this in it:
    
    #include "minunit.h"
    
    char *test_dlopen()
    {
    
        return NULL;
    }
    
    char *test_functions()
    {
    
        return NULL;
    }
    
    char *test_failures()
    {
    
        return NULL;
    }
    
    char *test_dlclose()
    {
    
        return NULL;
    }
    
    char *all_tests() {
        mu_suite_start();
    
        mu_run_test(test_dlopen);
        mu_run_test(test_functions);
        mu_run_test(test_failures);
        mu_run_test(test_dlclose);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

This code is demonstrating the RUN_TESTS macro in tests/minunit.h and how to use the other test runner macros. I have the actual test functions stubbed out so that you can see how to structure a unit test. I'll break this file down first:

libex29_tests.c:1
    Include the minunit.h framework.
libex29_tests.c:3-7
    A first test. Tests are structured so they take no arguments and return a char * which is NULL on success. This is important because the other macros will be used to return an error message to the test runner.
libex29_tests.c:9-25
    More tests that are the same as the first one.
libex29_tests.c:27
    The runner function that will control all the other tests. It has the same form as any other test case, but it gets configured with some additional gear.
libex29_tests.c:28
    Sets up some common stuff for a test with mu_suite_start.
libex29_tests.c:30
    This is how you say what test to run, using the mu_run_test macro.
libex29_tests.c:35
    After you say what tests to run, you then return NULL just like a normal test function.
libex29_tests.c:38
    Finally, you just use the big RUN_TESTS macro to wire up the main method with all the goodies and tell it to run the all_tests starter.

That's all there is to running a test, now you should try getting just this to run within the project skeleton. Here's what it looks like when I do it:
    
    $ make clean
    rm -rf build src/libex29.o tests/libex29_tests
    rm -f tests/tests.log 
    find . -name "*.gc*" -exec rm {} \;
    rm -rf `find . -name "*.dSYM" -print`
    $ make
    cc -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG  -fPIC 
        -c -o src/libex29.o src/libex29.c
    src/libex29.c: In function ‘fail_on_purpose’:
    src/libex29.c:42: warning: unused parameter ‘msg’
    ar rcs build/libYOUR_LIBRARY.a src/libex29.o
    ranlib build/libYOUR_LIBRARY.a
    cc -shared -o build/libYOUR_LIBRARY.so src/libex29.o
    cc -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG  build/libYOUR_LIBRARY.a 
        tests/libex29_tests.c   -o tests/libex29_tests
    sh ./tests/runtests.sh
    Running unit tests:
    ----
    RUNNING: ./tests/libex29_tests
    ALL TESTS PASSED
    Tests run: 4
    tests/libex29_tests PASS
    
    $
    

I first did a make clean and then I ran the build, which remade the template libYOUR_LIBRARY.a and libYOUR_LIBRARY.so files. Remember that you had to do this in the extra credit for Exercise 29, but just in case you didn't figure it out, here's the diff for the Makefile I'm using now:
    
    diff --git a/code/c-skeleton/Makefile b/code/c-skeleton/Makefile
    index 135d538..21b92bf 100644
    --- a/code/c-skeleton/Makefile
    +++ b/code/c-skeleton/Makefile
    @@ -9,9 +9,10 @@ TEST_SRC=$(wildcard tests/*_tests.c)
     TESTS=$(patsubst %.c,%,$(TEST_SRC))
    
     TARGET=build/libYOUR_LIBRARY.a
    +SO_TARGET=$(patsubst %.a,%.so,$(TARGET))
    
     # The Target Build
    -all: $(TARGET) tests
    +all: $(TARGET) $(SO_TARGET) tests
    
     dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
     dev: all
    @@ -21,6 +22,9 @@ $(TARGET): build $(OBJECTS)
             ar rcs $@ $(OBJECTS)
             ranlib $@
    
    +$(SO_TARGET): $(TARGET) $(OBJECTS)
    +       $(CC) -shared -o $@ $(OBJECTS)
    +
     build:
             @mkdir -p build
             @mkdir -p bin
    

With those changes you should be now building everything and you can finally fill in the remaining unit test functions:
    
    #include "minunit.h"
    #include <dlfcn.h>
    
    typedef int (*lib_function)(const char *data);
    char *lib_file = "build/libYOUR_LIBRARY.so";
    void *lib = NULL;
    
    int check_function(const char *func_to_run, const char *data, int expected)
    {
        lib_function func = dlsym(lib, func_to_run);
        check(func != NULL, "Did not find %s function in the library %s: %s", func_to_run, lib_file, dlerror());
    
        int rc = func(data);
        check(rc == expected, "Function %s return %d for data: %s", func_to_run, rc, data);
    
        return 1;
    error:
        return 0;
    }
    
    char *test_dlopen()
    {
        lib = dlopen(lib_file, RTLD_NOW);
        mu_assert(lib != NULL, "Failed to open the library to test.");
    
        return NULL;
    }
    
    char *test_functions()
    {
        mu_assert(check_function("print_a_message", "Hello", 0), "print_a_message failed.");
        mu_assert(check_function("uppercase", "Hello", 0), "uppercase failed.");
        mu_assert(check_function("lowercase", "Hello", 0), "lowercase failed.");
    
        return NULL;
    }
    
    char *test_failures()
    {
        mu_assert(check_function("fail_on_purpose", "Hello", 1), "fail_on_purpose should fail.");
    
        return NULL;
    }
    
    char *test_dlclose()
    {
        int rc = dlclose(lib);
        mu_assert(rc == 0, "Failed to close lib.");
    
        return NULL;
    }
    
    char *all_tests() {
        mu_suite_start();
    
        mu_run_test(test_dlopen);
        mu_run_test(test_functions);
        mu_run_test(test_failures);
        mu_run_test(test_dlclose);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

Hopefully by now you can figure out what's going on, since there's nothing new in this except for the check_function function. This is a common pattern where I see that I'll be doing a chunk of code repeatedly, and then simply automate it either by creating a function or a macro for it. In this case I'm going to run functions in the .so I load so I just made a little function to do it.

# Extra Credit

   This works but it's probably a bit messy. Clean the c-skeleton directory up so that it has all these files, but remove any of the code related to Exercise 29. You should be able to copy this directory over and kickstart new projects without much editing.
   Study the runtests.sh and go read about bash syntax so you know what it does. Think you could write a C version of this script?

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 


















