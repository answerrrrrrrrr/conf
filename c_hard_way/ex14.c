Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 14: Writing And Using Functions

Until now you've just used functions that are part of the stdio.h header file. In this exercise you will write some functions and use some other functions.
    
    #include <stdio.h>
    #include <ctype.h>
    
    // forward declarations
    int can_print_it(char ch);
    void print_letters(char arg[]);
    
    void print_arguments(int argc, char *argv[])
    {
        int i = 0;
    
        for(i = 0; i < argc; i++) {
            print_letters(argv[i]);
        }
    }
    
    void print_letters(char arg[])
    {
        int i = 0;
    
        for(i = 0; arg[i] != '\0'; i++) {
            char ch = arg[i];
    
            if(can_print_it(ch)) {
                printf("'%c' == %d ", ch, ch);
            }
        }
    
        printf("\n");
    }
    
    int can_print_it(char ch)
    {
        return isalpha(ch) || isblank(ch);
    }
    
    
    int main(int argc, char *argv[])
    {
        print_arguments(argc, argv);
        return 0;
    }
    

In this example you're creating functions to print out the characters and ASCII codes for any that are "alpha" or "blanks". Here's the breakdown:

ex14.c:2
    Include a new header file so we can gain access to isalpha and isblank.
ex14.c:5-6
    Tell C that you will be using some functions later in your program, without having to actually define them. This is a "forward declaration" and it solves the chicken-and-egg problem of needing to use a function before you've defined it.
ex14.c:8-15
    Define the print_arguments which knows how to print the same array of strings that main typically gets.
ex14.c:17-30
    Define the next function print_letters that is called by print_arguments and knows how to print each of the characters and their codes.
ex14.c:32-35
    Define can_print_it which simply returns the truth value (0 or 1) of isalpha(ch) || isblank(ch) back to its caller print_letters.
ex14.c:38-42
    Finally main simply calls print_arguments to make the whole chain of function calls go.

I shouldn't have to describe what's in each function because it's all things you've ran into before. What you should be able to see though is that I've simply defined functions the same way you've been defining main. The only difference is you have to help C out by telling it ahead of time if you're going to use functions it hasn't encountered yet in the file. That's what the "forward declarations" at the top do.

# What You Should See

To play with this program you just feed it different command line arguments, which get passed through your functions. Here's me playing with it to demonstrate:
    
    $ make ex14
    cc -Wall -g    ex14.c   -o ex14
    
    $ ./ex14
    'e' == 101 'x' == 120 
    
    $ ./ex14 hi this is cool
    'e' == 101 'x' == 120 
    'h' == 104 'i' == 105 
    't' == 116 'h' == 104 'i' == 105 's' == 115 
    'i' == 105 's' == 115 
    'c' == 99 'o' == 111 'o' == 111 'l' == 108 
    
    $ ./ex14 "I go 3 spaces"
    'e' == 101 'x' == 120 
    'I' == 73 ' ' == 32 'g' == 103 'o' == 111 ' ' == 32 ' ' == 32 's' == 115 'p' == 112 'a' == 97 'c' == 99 'e' == 101 's' == 115 
    $
    

The isalpha and isblank do all the work of figuring out if the given character is a letter or a blank. When I do the last run it prints everything but the '3' character, since that is a digit.

# How To Break It

There's two different kinds of "breaking" in this program:

   Confuse the compiler by removing the forward declarations so it complains about can_print_it and print_letters.
   When you call print_arguments inside main try adding 1 to argc so that it goes past the end of the argv array.

# Extra Credit

   Rework these functions so that you have fewer functions. For example, do you really need can_print_it?
   Have print_arguments figure how long each argument string is using the strlen function, and then pass that length to print_letters. Then, rewrite print_letters so it only processes this fixed length and doesn't rely on the '\0' terminator. You will need the #include <string.h> for this.
   Use man to lookup information on isalpha and isblank. Use the other similar functions to print out only digits or other characters.
   Go read about how different people like to format their functions. Never use the "K&R syntax" as it's antiquated and confusing, but understand what it's doing in case you run into someone who likes it.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















