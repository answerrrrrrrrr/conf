Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 12: If, Else-If, Else

Something common in every language is the if-statement, and C has one. Here's code that uses an if-statement to make sure you enter only 1 or 2 arguments:
    
    #include <stdio.h>
    
    int main(int argc, char *argv[])
    {
        int i = 0;
    
        if(argc == 1) {
            printf("You only have one argument. You suck.\n");
        } else if(argc > 1 && argc < 4) {
            printf("Here's your arguments:\n");
    
            for(i = 0; i < argc; i++) {
                printf("%s ", argv[i]);
            }
            printf("\n");
        } else {
            printf("You have too many arguments. You suck.\n");
        }
    
        return 0;
    }
    

The format for the if-statement is this:
    
    
    if(TEST) {
    CODE;
    } else if(TEST) {
    CODE;
    } else {
    CODE;
    }
    

This is like most other languages except for some specific C differences:

   As mentioned before, the TEST parts are false if they evaluate to 0, and true otherwise.
   You have to put parenthesis around the TEST elements, while some other languages let you skip that.
   You don't need the {} braces to enclose the code, but it is very bad form to not use them. The braces make it clear where one branch of code begins and ends. If you don't include it then obnoxious errors come up.

Other than that, they work like others do. You don't need to have either else if or else parts.

# What You Should See

This one is pretty simple to run and try out:
    
    $ make ex12
    cc -Wall -g    ex12.c   -o ex12
    $ ./ex12
    You only have one argument. You suck.
    $ ./ex12 one
    Here's your arguments:
    ./ex12 one 
    $ ./ex12 one two
    Here's your arguments:
    ./ex12 one two 
    $ ./ex12 one two three
    You have too many arguments. You suck.
    $
    

# How To Break It

This one isn't easy to break because it's so simple, but try messing up the tests in the if-statement.

   Remove the else at the end and it won't catch the edge case.
   Change the && to a || so you get an "or" instead of "and" test and see how that works.

# Extra Credit

   You were briefly introduced to &&, which does an "and" comparison, so go research online the different "boolean operators".
   Write a few more test cases for this program to see what you can come up with.
   Go back to Exercises 10 and 11, and use if-statements to make the loops exit early. You'll need the break statement to do that. Go read about it.
   Is the first test really saying the right thing? To you the "first argument" isn't the same first argument a user entered. Fix it.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















