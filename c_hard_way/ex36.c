Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 36: Safer Strings

I've already introduced you to the Better String library in Exercise 26 when we made devpkg. This exercise is designed to get you into using bstring from now on, why C's strings are an incredibly bad idea, and then have you change the liblcthw code to use bstring.

# Why C Strings Were A Horrible Idea

When people talk about problems with C, it's concept of a "string" is one of the top flaws. You've been using these extensively, and I've talked about the kinds of flaws they have, but there's not much that explains exactly why C strings are flawed and always will be. I'll try to explain that right now, but part of my explanation will just be that after decades of using C's strings there's enough evidence that they are just a bad idea.

It is impossible to confirm that any given C string is valid:

   A C string is invalid if it does not end in '\0'.
   Any loop that processes an invalid C string will loop infinitely (or, just buffer overflow).
   C strings do not have a known length, so the only way to check if it's terminated correctly is to loop through it.
   Therefore, it is not possible to validate a C string without possibly looping infinitely.

This is simple logic. You can't write a loop that checks if a C string is valid because invalid C strings cause loops to never terminate. That's it, and the only solution is to include the size. Once you know the size you can avoid the infinite loop problem. If you look at the two functions I showed you from Exercise 27 you can see this:
    
    void copy(char to[], char from[])
    {
        int i = 0;
    
        // while loop will not end if from isn't '\0' terminated
        while((to[i] = from[i]) != '\0') {
            ++i;
        }
    }
    
    int safercopy(int from_len, char *from, int to_len, char *to)
    {
        int i = 0;
        int max = from_len > to_len - 1 ? to_len - 1 : from_len;
    
        // to_len must have at least 1 byte
        if(from_len < 0 || to_len <= 0) return -1;
    
        for(i = 0; i < max; i++) {
            to[i] = from[i];
        }
    
        to[to_len - 1] = '\0';
    
        return i;
    }
    

Imagine you want to add a check to the copy function to confirm that the from string is valid. How would you do that? Why you'd write a loop that checked that the string ended in '\0'. Oh wait, if the string doesn't end in '\0' then how does the checking loop end? It doesn't. Checkmate.

No matter what you do, you can't check that a C string is valid without knowing the length of the underlying storage, and in this case the safercopy includes those lengths. This function doesn't have the same problem as it's loops will always terminate, even if you lie to it about the size, you still have to give it a finite size.

What the Better String library does is create a struct that always includes the length of the string's storage. Because the length is always available to a bstring then all of its operations can be safer. The loops will terminate, the contents can be validated, and it will not have this major flaw. The bstring library also comes with a ton of operations you need with strings, like splitting, formatting, searching, and they are most likely done right and safer.

There could be flaws in bstring, but it's been around a long time so those are probably minimal. They still find flaws in glibc so what's a programmer to do right?

# Using bstrlib

There's quite a few improved string libraries, but I like bstrlib because it fits in one file for the basics and has most of the stuff you need to deal with strings. You've already used it a bit, so in this exercise you'll go get the two files bstrlib.c and bstrlib.h from the Better String

Here's me doing this in the liblcthw project directory:
    
    $ mkdir bstrlib
    $ cd bstrlib/
    $ unzip ~/Downloads/bstrlib-05122010.zip
    Archive:  /Users/zedshaw/Downloads/bstrlib-05122010.zip
    ...
    $ ls
    bsafe.c             bstraux.c       bstrlib.h       bstrwrap.h      license.txt     test.cpp
    bsafe.h             bstraux.h       bstrlib.txt     cpptest.cpp     porting.txt     testaux.c
    bstest.c    bstrlib.c       bstrwrap.cpp    gpl.txt         security.txt
    $ mv bstrlib.h bstrlib.c ../src/lcthw/
    $ cd ../
    $ rm -rf bstrlib
    # make the edits
    $ vim src/lcthw/bstrlib.c
    $ make clean all
    ...
    $
    

On line 14 you seem me edit the bstrlib.c file to move it to a new location and to fix a bug on OSX. Here's the diff:
    
    25c25
    < #include "bstrlib.h"
    ---
    > #include <lcthw/bstrlib.h>
    2759c2759
    < #ifdef __GNUC__
    ---
    > #if defined(__GNUC__) && !defined(__APPLE__)
    

That is, change the include to be <lcthw/bstrlib.h>, and then fix one of the ifdef at line 2759.

# Learning The Library

This exercise is short and simply getting you ready for the remaining exercises that use the library. In the next two exercises I'll use bstrlib.c to create a Hashmap data structure.

You should now get familiar with this library by reading the header file, the implementations, and then write a tests/bstr_tests.c that tests out the following functions:

bfromcstr
    Create a bstring from a C style constant.
blk2bstr
    Same but give the length of the buffer.
bstrcpy
    Copy a bstring.
bassign
    Set one bstring to another.
bassigncstr
    Set a bstring to a C string's contents.
bassignblk
    Set a bstring to a C string but give the length.
bdestroy
    Destroy a bstring.
bconcat
    Concatenate one bstring onto another.
bstricmp
    Compare two bstrings returning the same result as strcmp.
biseq
    Tests if two bstrings are equal.
binstr
    Tells if one bstring is in another.
bfindreplace
    Find one bstring in another then replace it with a third.
bsplit
    How to split a bstring into a bstrList.
bformat
    Doing a format string, super handy.
blength
    Getting the length of a bstring.
bdata
    Getting the data from a bstring.
bchar
    Getting a char from a bstring.

Your test should try out all of these operations, and a few more that you find interesting from the header file. Make sure to run the test under valgrind to make sure you use the memory correctly.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 


















