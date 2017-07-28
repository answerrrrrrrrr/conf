Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 5: The Structure Of A C Program

You know how to use printf and have a couple basic tools at your disposal, so let's break down a simple C program line-by-line so you know how one is structured. In this program you're going to type in a few more things that you're unfamiliar with, and I'm going to lightly break them down. Then in the next few exercises we're going to work with these concepts.
    
    #include <stdio.h>
    
    /* This is a comment. */
    int main(int argc, char *argv[])
    {
        int distance = 100;
    
        // this is also a comment
        printf("You are %d miles away.\n", distance);
    
        return 0;
    }
    

Type this code in, make it run, and make sure you get no Valgrind errors. You probably won't but get in the habit of checking it.

# What You Should See

This has pretty boring output, but the point of this exercise is to analyze the code:
    
    $ make ex5
    cc -Wall -g    ex5.c   -o ex5
    $ ./ex5
    You are 100 miles away.
    $
    

# Breaking It Down

There's a few features of the C language in this code that you might have only slightly figured out while you were typing code. Let's break this down line-by-line quickly, and then we can do exercises to understand each part better:

ex5.c:1
    An include and it is the way to import the contents of one file into this source file. C has a convention of using .h extensions for "header" files, which then contain lists of functions you want to use in your program.
ex5.c:3
    This is a multi-line comment and you could put as many lines of text between the /* and closing */ characters as you want.
ex5.c:4
    A more complex version of the main function you've been using blindly so far. How C programs work is the operating system loads your program, and then runs the function named main. For the function to be totally complete it needs to return an int and take two parameters, an int for the argument count, and an array of char * strings for the arguments. Did that just fly over your head? Do not worry, we'll cover this soon.
ex5.c:5
    To start the body of any function you write a { character that indicates the beginning of a "block". In Python you just did a : and indented. In other languages you might have a begin or do word to start.
ex5.c:6
    A variable declaration and assignment at the same time. This is how you create a variable, with the syntax type name = value;. In C statements (except for logic) end in a ';' (semicolon) character.
ex5.c:8
    Another kind of comment, and it works like Python or Ruby comments where it starts at the // and goes until the end of the line.
ex5.c:9
    A call to your old friend printf. Like in many languages function calls work with the syntax name(arg1, arg2); and can have no arguments, or any number. The printf function is actually kind of weird and can take multiple arguments. We'll see that later.
ex5.c:11
    A return from the main function, which gives the OS your exit value. You may not be familiar with how Unix software uses return codes, so we'll cover that as well.
ex5.c:12
    Finally, we end the main function with a closing brace } character and that's the end of the program.

There's a lot of information in this break-down, so study it line-by-line and make sure you at least have a little grasp of what's going on. You won't know everything, but you can probably guess before we continue.

# Extra Credit

   For each line, write out the symbols you don't understand and see if you can guess what they mean. Write a little chart on paper with your guess that you can use to check later and see if you get it right.
   Go back to the source code from the previous exercises and do a similar break-down to see if you're getting it. Write down what you don't know and can't explain to yourself.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















