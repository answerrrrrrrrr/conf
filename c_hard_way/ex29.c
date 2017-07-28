Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 29: Libraries And Linking

A central part of any C program is the ability to link it to libraries that your operating system provides. Linking is how you get additional features for your program that someone else created and packaged on the system. You've been using some standard libraries that are automatically included, but I'm going to explain the different types of libraries and what they do.

First off, libraries are poorly designed in every programming language. I have no idea why, but it seems language designers think of linking as something they just slap on later. They are usually confusing, hard to deal with, can't do versioning right, and end up being linked differently everywhere.

C is no different, but the way linking and libraries are done in C is an artifact of how the Unix operating system and executable formats were designed years ago. Learning how C links things helps you understand how your OS works and how it runs your programs.

To start off there are two basic types of libraries:

static
    You've made one of these when you used ar and ranlib to create the libYOUR_LIBRARY.a in the last exercise. This kind of library is nothing more than a container for a set of .o object files and their functions, and you can treat it like one big .o file when building your programs.
dynamic
    These typically end in .so, .dll or about 1 million other endings on OSX depending on the version and who happened to be working that day. Seriously though, OSX adds .dylib, .bundle, and .framework with not much distinction between the three. These files are built and then placed in a common location. When you run your program the OS dynamically loads these files and links them to your program on the fly.

I tend to like static libraries for small to medium sized projects because they are easier to deal with and work on more operating systems. I also like to put all of the code I can into a static library so that I can then link it to unit tests and to the file programs as needed.

Dynamic libraries are good for larger systems, when space is tight, or if you have a large number of programs that use common functionality. In this case you don't want to statically link all of the code for the common features to every program, so you put it in a dynamic library so that it is loaded only once for all of them.

In the previous exercise I laid out how to make a static library (a .a file), and that's what I'll use in the rest of the book. In this exercise I'm going to show you how to make a simple .so library, and how to dynamically load it with the Unix dlopen system. I'll have you do this manually so that you understand everything that's actually happening, then the Extra Credit will be to use the c-skeleton skeleton to create it.

# Dynamically Loading A Shared Library

To do this I will create two source files. One will be used to make a libex29.so library, the other will be a program called ex29 that can load this library and run functions from it.
    
    #include <stdio.h>
    #include <ctype.h>
    #include "dbg.h"
    
    
    int print_a_message(const char *msg)
    {
        printf("A STRING: %s\n", msg);
    
        return 0;
    }
    
    
    int uppercase(const char *msg)
    {
        int i = 0;
    
        // BUG: \0 termination problems
        for(i = 0; msg[i] != '\0'; i++) {
            printf("%c", toupper(msg[i]));
        }
    
        printf("\n");
    
        return 0;
    }
    
    int lowercase(const char *msg)
    {
        int i = 0;
    
        // BUG: \0 termination problems
        for(i = 0; msg[i] != '\0'; i++) {
            printf("%c", tolower(msg[i]));
        }
    
        printf("\n");
    
        return 0;
    }
    
    int fail_on_purpose(const char *msg)
    {
        return 1;
    }
    

There's nothing fancy in there, although there's some bugs I'm leaving in on purpose to see if you've been paying attention. You'll fix those later.

What we want to do is use the functions dlopen, dlsym and dlclose to work with the above functions.
    
    #include <stdio.h>
    #include "dbg.h"
    #include <dlfcn.h>
    
    typedef int (*lib_function)(const char *data);
    
    
    int main(int argc, char *argv[])
    {
        int rc = 0;
        check(argc == 4, "USAGE: ex29 libex29.so function data");
    
        char *lib_file = argv[1];
        char *func_to_run = argv[2];
        char *data = argv[3];
    
        void *lib = dlopen(lib_file, RTLD_NOW);
        check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());
    
        lib_function func = dlsym(lib, func_to_run);
        check(func != NULL, "Did not find %s function in the library %s: %s", func_to_run, lib_file, dlerror());
    
        rc = func(data);
        check(rc == 0, "Function %s return %d for data: %s", func_to_run, rc, data);
    
        rc = dlclose(lib);
        check(rc == 0, "Failed to close %s", lib_file);
    
        return 0;
    
    error:
        return 1;
    }
    

I'll now break this down so you can see what's going on in this small bit of useful code:

ex29.c:5
    I'll use this function pointer definition later to call functions in the library. This is nothing new, but make sure you understand what it's doing.
ex29.c:17
    After the usual setup for a small program, I use the dlopen function to load up the library indicated by lib_file. This function returns a handle that we use later and works a lot like opening a file.
ex29.c:18
    If there's an error, I do the usual check and exit, but notice at then end that I'm using dlerror to find out what the library related error was.
ex29.c:20
    I use dlsym to get a function out of the lib by it's string name in func_to_run. This is the powerful part, since I'm dynamically getting a pointer to a function based on a string I got from the command line argv.
ex29.c:23
    I then call the func function that was returned, and check its return value.
ex29.c:26
    Finally, I close the library up just like I would a file. Usually you keep these open the whole time the program is running, so closing at the end isn't as useful, but I'm demonstrating it here.

# What You Should See

Now that you know what this file does, here's a shell session of me building the libex29.so, ex29 and then working with it. Follow along so you learn how these things are built manually.
    
    # compile the lib file and make the .so
    $ cc -c libex29.c -o libex29.o
    $ cc -shared -o libex29.so libex29.o
    
    # make the loader program
    $ cc -Wall -g -DNDEBUG ex29.c -ldl -o ex29
    
    # try it out with some things that work
    $ ex29 ./libex29.so print_a_message "hello there"
    -bash: ex29: command not found
    $ ./ex29 ./libex29.so print_a_message "hello there"
    A STRING: hello there
    $ ./ex29 ./libex29.so uppercase "hello there"
    HELLO THERE
    $ ./ex29 ./libex29.so lowercase "HELLO tHeRe"
    hello there
    $ ./ex29 ./libex29.so fail_on_purpose "i fail"
    [ERROR] (ex29.c:23: errno: None) Function fail_on_purpose return 1 for data: i fail
    
    # try to give it bad args
    $ ./ex29 ./libex29.so fail_on_purpose
    [ERROR] (ex29.c:11: errno: None) USAGE: ex29 libex29.so function data
    
    # try calling a function that is not there
    $ ./ex29 ./libex29.so adfasfasdf asdfadff
    [ERROR] (ex29.c:20: errno: None) Did not find adfasfasdf 
      function in the library libex29.so: dlsym(0x1076009b0, adfasfasdf): symbol not found
    
    # try loading a .so that is not there
    $ ./ex29 ./libex.so adfasfasdf asdfadfas
    [ERROR] (ex29.c:17: errno: No such file or directory) Failed to open
        the library libex.so: dlopen(libex.so, 2): image not found
    $
    

One thing that you may run into is that every OS, every version of every OS, and every compiler on every version of every OS, seems to want to change the way you build a shared library every other month that some new programmer thinks it's wrong. If the line I use to make the libex29.so file is wrong, then let me know and I'll add some comments for other platforms.

Note

Sometimes you'll do what you think is normal and run this command cc -Wall -g -DNDEBUG -ldl ex29.c -o ex29 thinking everthing will work, but nope. You see, on some platforms the order of where libraries goes makes them work or not, and for no real reason. On Debian or Ubuntu you have to do cc -Wall -g -DNDEBUG ex29.c -ldl -o ex29 for no reason at all. It's just the way it is, so since this works on OSX I'm doing it here, but in the future, if you link against a dynamic library and it can't find a function, try shuffling things around.

The irritation here is there is an actual platform difference on nothing more than order of command line arguments. On no rational planet should putting an -ldl at one position be different from another. It's an option, and having to know these things is incredibly annoying.

# How To Break It

Open libex29.so and edit it with an editor that can handle binary files. Change a couple bytes, then close it. Try to see if you can get the dlopen function to load it even though you've corrupted it.

# Extra Credit

   Were you paying attention to the bad code I have in the libex29.c functions? See how, even though I use a for-loop they still check for '\0' endings? Fix this so the functions always take a length for the string to work with inside the function.
   Take the c-skeleton skeleton, and create a new project for this exercise. Put the libex29.c file in the src/ directory. Change the Makefile so that it builds this as build/libex29.so.
   Take the ex29.c file and put it in tests/ex29_tests.c so that it runs as a unit test. Make this all work, which means you have to change it so that it loads the build/libex29.so file and runs tests similar to what I did manually above.
   Read the man dlopen documentation and read about all the related functions. Try some of the other options to dlopen beside RTLD_NOW.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















