Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 2: Make Is Your Python Now

In Python you ran programs by just typing python and the code you wanted to run. The Python interpreter would just run them, and import any other libraries and things you needed on the fly as it ran. C is a different beast completely where you have to compile your source files and manually stitch them together into a binary that can run on its own. Doing this manually is a pain, and in the last exercise you just ran make to do it.

In this exercise, you're going to get a crash course in GNU make, and you'll be learning to use it as you learn C. Make will for the rest of this book, be your Python. It will build your code, and run your tests, and set things up and do all the stuff for you that Python normally does.

The difference is, I'm going to show you smarter Makefile wizardry, where you don't have to specify every stupid little thing about your C program to get it to build. I won't do that in this exercise, but after you've been using "baby make" for a while, I'll show you "master make".

# Using Make

The first stage of using make is to just use it to build programs it already knows how to build. Make has decades of knowledge on building a wide variety of files from other files. In the last exercise you did this already using commands like this:
    
    
    $ make ex1
    $ make ex1
    # or this one too
    $ CFLAGS="-Wall" make ex1
    

In the first command you're telling make, "I want a file named ex1 to be created." Make then does the following:

   Does the file ex1 exist already?
   No. Ok, is there another file that starts with ex1?
   Yes, it's called ex1.c. Do I know how to build .c files?
   Yes, I run this command cc ex1.c   -o ex1 to build them.
   I shall make you one ex1 by using cc to build it from ex1.c.

The second command in the listing above is a way to pass "modifiers" to the make command. If you're not familiar with how the Unix shell works, you can create these "environment variables" which will get picked up by programs you run. Sometimes you do this with a command like export CFLAGS="-Wall" depending on the shell you use. You can however also just put them before the command you want to run, and that environment variable will be set only while that command runs.

In this example I did CFLAGS="-Wall" make ex1 so that it would add the command line option -Wall to the cc command that make normally runs. That command line option tells the compiler cc to report all warnings (which in a sick twist of fate isn't actually all the warnings possible).

You can actually get pretty far with just that way of using make, but let's get into making a Makefile so you can understand make a little better. To start off, create a file with just this in it:
    
    CFLAGS=-Wall -g
    
    clean:
       rm -f ex1
    

Save this file as Makefile in your current directory. Make automatically assumes there's a file called Makefile and will just run it. Also, WARNING: Make sure you are only entering TAB characters, not mixtures of TAB and spaces.

This Makefile is showing you some new stuff with make. First we set CFLAGS in the file so we never have to set it again, as well as adding the -g flag to get debugging. Then we have a section named clean which tells make how to clean up our little project.

Make sure it's in the same directory as your ex1.c file, and then run these commands:
    
    
    $ make clean
    $ make clean
    $ make ex1
    

# What You Should See

If that worked then you should see this:
    
    $ make clean
    rm -f ex1
    $ make ex1
    cc -Wall -g    ex1.c   -o ex1
    ex1.c: In function 'main':
    ex1.c:3: warning: implicit declaration of function 'puts'
    $
    

Here you can see that I'm running make clean which tells make to run our clean target. Go look at the Makefile again and you'll see that under this I indent and then I put the shell commands I want make to run for me. You could put as many commands as you wanted in there, so it's a great automation tool.

Note

If you fixed ex1.c to have #include <stdio.h> then your output will not have the warning (which should really be an error) about puts. I have the error here because I didn't fix it.

Notice also that, even though we don't mention ex1 in the Makefile, make still knows how to build it plus use our special settings.

# How To Break It

That should be enough to get you started, but first let's break this make file in a particular way so you can see what happens. Take the line rm -f ex1 and dedent it (move it all the way left) so you can see what happens. Rerun make clean and you should get something like this:
    
    
    $ make clean
    $ make clean
    Makefile:4: *** missing separator.  Stop.
    

Always remember to indent, and if you get weird errors like this then double check you're consistently using tab characters since some make variants are very picky.

# Extra Credit

   Create an all: ex1 target that will build ex1 with just the command make.
   Read man make to find out more information on how to run it.
   Read man cc to find out more information on what the flags -Wall and -g do.
   Research Makefiles online and see if you can improve this one even more.
   Find a Makefile in another C project and try to understand what it's doing.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















