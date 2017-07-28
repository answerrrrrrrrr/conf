Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 4: Introducing Valgrind

It's time to learn about another tool you will live and die by as you learn C called Valgrind. I'm introducing Valgrind to you now because you're going to use it from now on in the "How To Break It" sections of each exercise. Valgrind is a program that runs your programs, and then reports on all of the horrible mistakes you made. It's a wonderful free piece of software that I use constantly while I write C code.

Remember in the last exercise that I told you to break your code by removing one of the arguments to printf? It printed out some funky results, but I didn't tell you why it printed those results out. In this exercise we're going to use Valgrind to find out why.

Note

These first few exercises are mixing some essential tools the rest of the book needs with learning a little bit of code. The reason is that most of the folks who read this book are not familiar with compiled languages, and definitely not with automation and helpful tools. By getting you to use make and Valgrind right now I can then use them to teach you C faster and help you find all your bugs early.

After this exercise we won't do many more tools, it'll be mostly code and syntax for a while. But, we'll also have a few tools we can use to really see what's going on and get a good understanding of common mistakes and problems.

# Installing Valgrind

You could install Valgrind with the package manager for your OS, but I want you to learn to install things from source. This involves the following process:

   Download a source archive file to get the source.
   Unpack the archive to extract the files onto your computer.
   Run ./configure to setup build configurations.
   Run make to make it build, just like you've been doing.
   Run sudo make install to install it onto your computer.

Here's a script of me doing this very process, which I want you to try to replicate:
    
    # 1) Download it (use wget if you don't have curl)
    curl -O http://valgrind.org/downloads/valgrind-3.6.1.tar.bz2
    
    # use md5sum to make sure it matches the one on the site
    md5sum valgrind-3.6.1.tar.bz2
    
    # 2) Unpack it.
    tar -xjvf valgrind-3.6.1.tar.bz2
    
    # cd into the newly created directory
    cd valgrind-3.6.1
    
    # 3) configure it
    ./configure
    
    # 4) make it
    make
    
    # 5) install it (need root)
    sudo make install
    

Follow this, but obviously update it for new Valgrind versions. If it doesn't build then try digging into why as well.

# Using Valgrind

Using Valgrind is easy, you just run valgrind theprogram and it runs your program, then prints out all the errors your program made while it was running. In this exercise we'll break down one of the error outputs and you can get an instant crash course in "Valgrind hell". Then we'll fix the program.

First, here's a purposefully broken version of the ex3.c code for you to build, now called ex4.c. For practice, type it in again:
    
    #include <stdio.h>
    
    /* Warning: This program is wrong on purpose. */
    
    int main()
    {
        int age = 10;
        int height;
    
        printf("I am %d years old.\n");
        printf("I am %d inches tall.\n", height);
    
        return 0;
    }
    

You'll see it's the same except I've made two classic mistakes:

   I've failed to initialize the height variable.
   I've forgot to give the first printf the age variable.

# What You Should See

Now we will build this just like normal, but instead of running it directly, we'll run it with Valgrind (see Source: "Building and running ex4.c with Valgrind"):
    
    $ make ex4
    cc -Wall -g    ex4.c   -o ex4
    ex4.c: In function 'main':
    ex4.c:10: warning: too few arguments for format
    ex4.c:7: warning: unused variable 'age'
    ex4.c:11: warning: 'height' is used uninitialized in this function
    $ valgrind ./ex4
    ==3082== Memcheck, a memory error detector
    ==3082== Copyright (C) 2002-2010, and GNU GPL'd, by Julian Seward et al.
    ==3082== Using Valgrind-3.6.0.SVN-Debian and LibVEX; rerun with -h for copyright info
    ==3082== Command: ./ex4
    ==3082== 
    I am -16775432 years old.
    ==3082== Use of uninitialised value of size 8
    ==3082==    at 0x4E730EB: _itoa_word (_itoa.c:195)
    ==3082==    by 0x4E743D8: vfprintf (vfprintf.c:1613)
    ==3082==    by 0x4E7E6F9: printf (printf.c:35)
    ==3082==    by 0x40052B: main (ex4.c:11)
    ==3082== 
    ==3082== Conditional jump or move depends on uninitialised value(s)
    ==3082==    at 0x4E730F5: _itoa_word (_itoa.c:195)
    ==3082==    by 0x4E743D8: vfprintf (vfprintf.c:1613)
    ==3082==    by 0x4E7E6F9: printf (printf.c:35)
    ==3082==    by 0x40052B: main (ex4.c:11)
    ==3082== 
    ==3082== Conditional jump or move depends on uninitialised value(s)
    ==3082==    at 0x4E7633B: vfprintf (vfprintf.c:1613)
    ==3082==    by 0x4E7E6F9: printf (printf.c:35)
    ==3082==    by 0x40052B: main (ex4.c:11)
    ==3082== 
    ==3082== Conditional jump or move depends on uninitialised value(s)
    ==3082==    at 0x4E744C6: vfprintf (vfprintf.c:1613)
    ==3082==    by 0x4E7E6F9: printf (printf.c:35)
    ==3082==    by 0x40052B: main (ex4.c:11)
    ==3082== 
    I am 0 inches tall.
    ==3082== 
    ==3082== HEAP SUMMARY:
    ==3082==     in use at exit: 0 bytes in 0 blocks
    ==3082==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
    ==3082== 
    ==3082== All heap blocks were freed -- no leaks are possible
    ==3082== 
    ==3082== For counts of detected and suppressed errors, rerun with: -v
    ==3082== Use --track-origins=yes to see where uninitialised values come from
    ==3082== ERROR SUMMARY: 4 errors from 4 contexts (suppressed: 4 from 4)
    $
    

Note

If you run valgrind and it says something like by 0x4052112: (below main) (libc-start.c:226) instead of a line number in main.c then add run your valgrind command like this valgrind --track-origins=yes ./ex4 to make it work. For some reason the Debian or Ubuntu version of valgrind does this but not other versions.

This one is huge because Valgrind is telling you exactly where every problem in your program is. Starting at the top here's what you're reading, line by line (line numbers are on the left so you can follow):

1
    You do the usual make ex4 and that builds it. Make sure the cc command you see is the same and has the -g option or your Valgrind output won't have line numbers.
2-6
    Notice that the compiler is also yelling at you about this source file and it warns you that you have "too few arguments for format". That's where you forgot to include the age variable.
7
    Then you run your program using valgrind ./ex4.
8
    

Then Valgrind goes crazy and yells at you for:

14-18
    On line main (ex4.c:11) (read as "in the main function in file ex4.c at line 11) you have "Use of uninitialised value of size 8". You find this by looking at the error, then you see what's called a "stack trace" right under that. The line to look at first (ex4.c:11) is the bottom one, and if you don't see what's going wrong then you go up, so you'd try printf.c:35. Typically it's the bottom most line that matters (in this case, on line 18).
20-24
    Next error is yet another one on line ex4.c:11 in the main function. Valgrind hates this line. This error says that some kind of if-statement or while-loop happened that was based on an uninitialized variable, in this case height.
25-35
    The remaining errors are more of the same because the variable keeps getting used.
37-46
    Finally the program exits and Valgrind tells you a summary of how bad your program is.

That is quite a lot of information to take in, but here's how you deal with it:

   Whenever you run your C code and get it working, rerun it under Valgrind to check it.
   For each error that you get, go to the source:line indicated and fix it. You may have to search online for the error message to figure out what it means.
   Once your program is "Valgrind pure" then it should be good, and you have probably learned something about how you write code.

In this exercise I'm not expecting you to fully grasp Valgrind right away, but instead get it installed and learn how to use it real quick so we can apply it to all the later exercises.

# Extra Credit

   Fix this program using Valgrind and the compiler as your guide.
   Read up on Valgrind on the internet.
   Download other software and build it by hand. Try something you already use but never built for yourself.
   Look at how the Valgrind source files are laid out in the source directory and read its Makefile. Don't worry, none of that makes sense to me either.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















