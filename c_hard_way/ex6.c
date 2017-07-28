Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 6: Types Of Variables

You should be getting a grasp of how a simple C program is structured, so let's do the next simplest thing which is making some variables of different types:
    
    #include <stdio.h>
    
    int main(int argc, char *argv[])
    {
        int distance = 100;
        float power = 2.345f;
        double super_power = 56789.4532;
        char initial = 'A';
        char first_name[] = "Zed";
        char last_name[] = "Shaw";
    
        printf("You are %d miles away.\n", distance);
        printf("You have %f levels of power.\n", power);
        printf("You have %f awesome super powers.\n", super_power);
        printf("I have an initial %c.\n", initial);
        printf("I have a first name %s.\n", first_name);
        printf("I have a last name %s.\n", last_name);
        printf("My whole name is %s %c. %s.\n",
                first_name, initial, last_name);
    
        return 0;
    }
    

In this program we're declaring variables of different types and then printing them with different printf format strings.

# What You Should See

Your output should look like mine, and you can start to see how the format strings for C are similar to Python and other languages. They've been around for a long time.
    
    $ make ex6
    cc -Wall -g    ex6.c   -o ex6
    $ ./ex6
    You are 100 miles away.
    You have 2.345000 levels of power.
    You have 56789.453200 awesome super powers.
    I have an initial A.
    I have a first name Zed.
    I have a last name Shaw.
    My whole name is Zed A. Shaw.
    $
    

What you can see is we have a set of "types", which are ways of telling the C compiler what each variable should represent, and then format strings to match different types. Here's the breakdown of how they match up:

Integers
    You declare Integers with the int keyword, and print them with %d.
Floating Point
    Declared with float or double depending on how big they need to be (double is bigger), and printed with %f.
Character
    Declared with char, written with a ' (single-quote) character around the char, and then printed with %c.
String (Array of Characters)
    Declared with char name[], written with " characters, and printed with %s.

You'll notice that C makes a distinction between single-quote for char and double-quote for char[] or strings.

Note

When talking about C types, I will typically write in English char[] instead of the whole char SOMENAME[]. This is not valid C code, just a simpler way to talk about types when writing English.

# How To Break It

You can easily break this program by passing the wrong thing to the printf statements. For example, if you take the line that prints my name, but put the initial variable before the first_name in the arguments, you'll get a bug. Make that change and the compiler will yell at you, then when you run it you might get a "Segmentation fault" like I did:
    
    $ make ex6
    cc -Wall -g    ex6.c   -o ex6
    ex6.c: In function 'main':
    ex6.c:19: warning: format '%s' expects type 'char *', but argument 2 has type 'int'
    ex6.c:19: warning: format '%c' expects type 'int', but argument 3 has type 'char *'
    $ ./ex6
    You are 100 miles away.
    You have 2.345000 levels of power.
    You have 56789.453125 awesome super powers.
    I have an initial A.
    I have a first name Zed.
    I have a last name Shaw.
    Segmentation fault
    $
    

Run this change under Valgrind too to see what it tells you about the error "Invalid read of size 1".

# Extra Credit

   Come up with other ways to break this C code by changing the printf, then fix them.
   Go search for "printf formats" and try using a few of the more exotic ones.
   Research how many different ways you can write a number. Try octal, hexadecimal, and others you can find.
   Try printing an empty string that's just "".

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















