Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 11: While-Loop And Boolean Expressions

You've had your first taste of how C does loops, but the boolean expression i < argc might have not been clear to you. Let me explain something about it before we see how a while-loop works.

In C, there's not really a "boolean" type, and instead any integer that's 0 is "false" and otherwise it's "true". In the last exercise the expression i < argc actually resulted in 1 or 0, not an explicit True or False like in Python. This is another example of C being closer to how a computer works, because to a computer truth values are just integers.

Now you'll take and implement the same program from the last exercise but use a while-loop instead. This will let you compare the two so you can see how one is related to another.
    
    #include <stdio.h>
    
    int main(int argc, char *argv[])
    {
        // go through each string in argv
    
        int i = 0;
        while(i < argc) {
            printf("arg %d: %s\n", i, argv[i]);
            i++;
        }
    
        // let's make our own array of strings
        char *states[] = {
            "California", "Oregon",
            "Washington", "Texas"
        };
    
        int num_states = 4;
        i = 0;  // watch for this
        while(i < num_states) {
            printf("state %d: %s\n", i, states[i]);
            i++;
        }
    
        return 0;
    }
    

You can see from this that a while-loop is simpler:
    
    
    while(TEST) {
    CODE;
    }
    

It simply runs the CODE as long as TEST is true (1). This means that to replicate how the for-loop works we need to do our own initializing and incrementing of i.

# What You Should See

The output is basically the same, so I just did it a little different so you can see another way it runs.
    
    $ make ex11
    cc -Wall -g    ex11.c   -o ex11
    $ ./ex11
    arg 0: ./ex11
    state 0: California
    state 1: Oregon
    state 2: Washington
    state 3: Texas
    $
    $ ./ex11 test it
    arg 0: ./ex11
    arg 1: test
    arg 2: it
    state 0: California
    state 1: Oregon
    state 2: Washington
    state 3: Texas
    $
    

# How To Break It

In your own code you should favor for-loop constructs over while-loop because a for-loop is harder to break. Here's a few common ways:

   Forget to initialize the first int i; so have it loop wrong.
   Forget to initialize the second loop's i so that it retains the value from the end of the first loop. Now your second loop might or might not run.
   Forget to do a i++ increment at the end of the loop and you get a "forever loop", one of the dreaded problems of the first decade or two of programming.

# Extra Credit

   Make these loops count backward by using i-- to start at argc and count down to 0. You may have to do some math to make the array indexes work right.
   Use a while loop to copy the values from argv into states.
   Make this copy loop never fail such that if there's too many argv elements it won't put them all into states.
   Research if you've really copied these strings. The answer may surprise and confuse you though.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















