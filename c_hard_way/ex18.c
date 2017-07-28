Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 18: Pointers To Functions

Functions in C are actually just pointers to a spot in the program where some code exists. Just like you've been creating pointers to structs, strings, and arrays, you can point a pointer at a function too. The main use for this is to pass "callbacks" to other functions, or to simulate classes and objects. In this exercise we'll do some callbacks, and in the next one we'll make a simple object system.

The format of a function pointer goes like this:

int (*POINTER_NAME)(int a, int b)

A way to remember how to write one is to do this:

   Write a normal function declaration: int callme(int a, int b)
   Wrap function name with pointer syntax: int (*callme)(int a, int b)
   Change the name to the pointer name: int (*compare_cb)(int a, int b)

The key thing to remember is, when you're done with this, the variable name for the pointer is called compare_cb and then you use it just like it's a function. This is similar to how pointers to arrays can be used just like the arrays they point to. Pointers to functions can be used like the functions they point to but with a different name.
    
    
    int (*tester)(int a, int b) = sorted_order;
    printf("TEST: %d is same as %d\n", tester(2, 3), sorted_order(2, 3));
    

This will work even if the function pointer returns a pointer to something:

   Write it: char *make_coolness(int awesome_levels)
   Wrap it: char *(*make_coolness)(int awesome_levels)
   Rename it: char *(*coolness_cb)(int awesome_levels)

The next problem to solve with using function pointers is that it's hard to give them as parameters to a function, like when you want to pass the function callback to another function. The solution to this is to use typedef which is a C keyword for making new names for other more complex types. The only thing you need to do is put typedef before the same function pointer syntax, and then after that you can use the name like it's a type. I demonstrate this in the following exercise code:
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>
    #include <errno.h>
    #include <string.h>
    
    /** Our old friend die from ex17. */
    void die(const char *message)
    {
        if(errno) {
            perror(message);
        } else {
            printf("ERROR: %s\n", message);
        }
    
        exit(1);
    }
    
    // a typedef creates a fake type, in this
    // case for a function pointer
    typedef int (*compare_cb)(int a, int b);
    
    /**
     * A classic bubble sort function that uses the 
     * compare_cb to do the sorting. 
     */
    int *bubble_sort(int *numbers, int count, compare_cb cmp)
    {
        int temp = 0;
        int i = 0;
        int j = 0;
        int *target = malloc(count * sizeof(int));
    
        if(!target) die("Memory error.");
    
        memcpy(target, numbers, count * sizeof(int));
    
        for(i = 0; i < count; i++) {
            for(j = 0; j < count - 1; j++) {
                if(cmp(target[j], target[j+1]) > 0) {
                    temp = target[j+1];
                    target[j+1] = target[j];
                    target[j] = temp;
                }
            }
        }
    
        return target;
    }
    
    int sorted_order(int a, int b)
    {
        return a - b;
    }
    
    int reverse_order(int a, int b)
    {
        return b - a;
    }
    
    int strange_order(int a, int b)
    {
        if(a == 0 || b == 0) {
            return 0;
        } else {
            return a % b;
        }
    }
    
    /** 
     * Used to test that we are sorting things correctly
     * by doing the sort and printing it out.
     */
    void test_sorting(int *numbers, int count, compare_cb cmp)
    {
        int i = 0;
        int *sorted = bubble_sort(numbers, count, cmp);
    
        if(!sorted) die("Failed to sort as requested.");
    
        for(i = 0; i < count; i++) {
            printf("%d ", sorted[i]);
        }
        printf("\n");
    
        free(sorted);
    }
    
    
    int main(int argc, char *argv[])
    {
        if(argc < 2) die("USAGE: ex18 4 3 1 5 6");
    
        int count = argc - 1;
        int i = 0;
        char **inputs = argv + 1;
    
        int *numbers = malloc(count * sizeof(int));
        if(!numbers) die("Memory error.");
    
        for(i = 0; i < count; i++) {
            numbers[i] = atoi(inputs[i]);
        }
    
        test_sorting(numbers, count, sorted_order);
        test_sorting(numbers, count, reverse_order);
        test_sorting(numbers, count, strange_order);
    
        free(numbers);
    
        return 0;
    }
    

In this program you're creating a dynamic sorting algorithm that can sort an array of integers using a comparison callback. Here's the breakdown of this program so you can clearly understand it:

ex18.c:1-6
    The usual includes needed for all the functions we call.
ex18.c:7-17
    This is the die function from the previous exercise which I'll use to do error checking.
ex18.c:21
    This is where the typedef is used, and later I use compare_cb like it's a type similar to int or char in bubble_sort and test_sorting.
ex18.c:27-49
    

A bubble sort implementation, which is a very inefficient way to sort some integers. This function contains:

ex18.c:27
    Here's where I use the typedef for compare_cb as the last parameter cmp. This is now a function that will return a comparison between two integers for sorting.
ex18.c:29-34
    The usual creation of variables on the stack, followed by a new array of integers on the heap using malloc. Make sure you understand what count * sizeof(int) is doing.
ex18.c:38
    The outer-loop of the bubble sort.
ex18.c:39
    The inner-loop of the bubble sort
ex18.c:40
    Now I call the cmp callback just like it's a normal function, but instead of being the name of something we defined, it's just a pointer to it. This lets the caller pass in anything they want as long as it matches the "signature" of the compare_cb typedef.
ex18.c:41-43
    The actual swapping operation a bubble sort needs to do what it does.
ex18.c:48
    Finally return the newly created and sorted result array target.
ex18.c:51-68
    Three different versions of the compare_cb function type, which needs to have the same definition as the typedef we created. The C compiler will complain to you if you get this wrong and say the types don't match.
ex18.c:74-87
    This is a tester for the bubble_sort function. You can see now how I'm also using compare_cb to then pass to bubble_sort demonstrating how these can be passed around like any other pointers.
ex18.c:90-103
    A simple main function that sets up an array based on integers you pass on the command line, then calls the test_sorting function.
ex18.c:105-107
    Finally, you get to see how the compare_cb function pointer typedef is used. I simply call test_sorting but give it the name of sorted_order, reverse_order, and strange_order as the function to use. The C compiler then finds the address of those functions, and makes it a pointer for test_sorting to use. If you look at test_sorting you'll see it then passes each of these to bubble_sort but it actually has no idea what they do, only that they match the compare_cb prototype and should work.
ex18.c:109
    Last thing we do is free up the array of numbers we made.

# What You Should See

Running this program is simple, but try different combinations of numbers, and try even non-numbers to see what it does.
    
    $ make ex18
    cc -Wall -g    ex18.c   -o ex18
    $ ./ex18 4 1 7 3 2 0 8
    0 1 2 3 4 7 8 
    8 7 4 3 2 1 0 
    3 4 2 7 1 0 8 
    $
    

# How To Break It

I'm going to have you do something kind of weird to break this. These function pointers are pointers like every other pointer, so they point at blocks of memory. C has this ability to take one pointer and convert it to another so you can process the data in different ways. It's usually not necessary, but to show you how to hack your computer, I want you to add this at the end of test_sorting:
    
    
    unsigned char *data = (unsigned char *)cmp;
    
    for(i = 0; i < 25; i++) {
    printf("%02x:", data[i]);
    }
    printf("\n");
    

This loop is sort of like converting your function to a string and then printing out it's contents. This won't break your program unless the CPU and OS you're on has a problem with you doing this. What you'll see is a string of hexadecimal numbers after it prints the sorted array:
    
    
    55:48:89:e5:89:7d:fc:89:75:f8:8b:55:fc:8b:45:f8:29:d0:c9:c3:55:48:89:e5:89:
    

That should be the raw assembler byte code of the function itself, and you should see they start the same, but then have different endings. It's also possible that this loop isn't getting all of the function or is getting too much and stomping on another piece of the program. Without more analysis you wouldn't know.

# Extra Credit

   Get a hex editor and open up ex18, then find this sequence of hex digits that start a function to see if you can find the function in the raw program.
   Find other random things in your hex editor and change them. Rerun your program and see what happens. Changing strings you find are the easiest things to change.
   Pass in the wrong function for the compare_cb and see what the C compiler complains about.
   Pass in NULL and watch your program seriously bite it. Then run Valgrind and see what that reports.
   Write another sorting algorithm, then change test_sorting so that it takes both an arbitrary sort function and the sort function's callback comparison. Use it to test both of your algorithms.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















