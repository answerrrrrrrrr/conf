Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 3: Formatted Printing

Keep that Makefile around since it'll help you spot errors and we'll be adding to it when we need to automate more things.

Many programming languages use the C way of formatting output, so let's try it:
    
    #include <stdio.h>
    
    int main()
    {
        int age = 10;
        int height = 72;
    
        printf("I am %d years old.\n", age);
        printf("I am %d inches tall.\n", height);
    
        return 0;
    }
    

Once you have that, do the usual make ex3 to build it and run it. Make sure you fix all warnings.

This exercise has a whole lot going on in a small amount of code so let's break it down:

   First you're including another "header file" called stdio.h. This tells the compiler that you're going to use the "standard Input/Output functions". One of those is printf.
   Then you're using a variable named age and setting it to 10.
   Next you're using a variable height and setting it to 72.
   Then you use the printf function to print the age and height of the tallest 10 year old on the planet.
   In the printf you'll notice you're passing in a string, and it's a format string like in many other languages.
   After this format string, you put the variables that should be "replaced" into the format string by printf.

The result of doing this is you are handing printf some variables and it is constructing a new string then printing that new string to the terminal.

# What You Should See

When you do the whole build you should see something like this:
    
    $ make ex3
    cc -Wall -g    ex3.c   -o ex3
    $ ./ex3
    I am 10 years old.
    I am 72 inches tall.
    $
    

Pretty soon I'm going to stop telling you to run make and what the build looks like, so please make sure you're getting this right and that it's working.

# External Research

In the Extra Credit section of each exercise I may have you go find information on your own and figure things out. This is an important part of being a self-sufficient programmer. If you constantly run to ask someone a question before trying to figure it out first then you never learn to solve problems independently. This leads to you never building confidence in your skills and always needing someone else around to do your work.

The way you break this habit is to force yourself to try to answer your own questions first, and to confirm that your answer is right. You do this by trying to break things, experimenting with your possible answer, and doing your own research.

For this exercise I want you to go online and find out all of the printf escape codes and format sequences. Escape codes are \n or \t that let you print a newline or tab (respectively). Format sequences are the %s or %d that let you print a string or a integer. Find all of the ones available, how you can modify them, and what kind of "precisions" and widths you can do.

From now on, these kinds of tasks will be in the Extra Credit and you should do them.

# How To Break It

Try a few of these ways to break this program, which may or may not cause it to crash on your computer:

   Take the age variable out of the first printf call then recompile. You should get a couple of warnings.
   Run this new program and it will either crash, or print out a really crazy age.
   Put the printf back the way it was, and then don't set age to an initial value by changing that line to int age; then rebuild and run again.
    
    # edit ex3.c to break printf
    $ make ex3
    cc -Wall -g    ex3.c   -o ex3
    ex3.c: In function 'main':
    ex3.c:8: warning: too few arguments for format
    ex3.c:5: warning: unused variable 'age'
    $ ./ex3
    I am -919092456 years old.
    I am 72 inches tall.
    # edit ex3.c again to fix printf, but don't init age
    $ make ex3
    cc -Wall -g    ex3.c   -o ex3
    ex3.c: In function 'main':
    ex3.c:8: warning: 'age' is used uninitialized in this function
    $ ./ex3
    I am 0 years old.
    I am 72 inches tall.
    $
    

# Extra Credit

   Find as many other ways to break ex3.c as you can.
   Run man 3 printf and read about the other '%' format characters you can use. These should look familiar if you used them in other languages (printf is where they come from).
   Add ex3 to your Makefile's all list. Use this to make clean all and build all your exercises so far.
   Add ex3 to your Makefile's clean list as well. Now use make clean will remove it when you need to.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















