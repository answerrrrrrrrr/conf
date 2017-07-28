Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 7: More Variables, Some Math

Let's get familiar with more things you can do with variables by declaring various ints, floats, chars, and doubles. We'll then use these in various math expressions so you get introduced to C's basic math.
    
    #include <stdio.h>
    
    int main(int argc, char *argv[])
    {
        int bugs = 100;
        double bug_rate = 1.2;
    
        printf("You have %d bugs at the imaginary rate of %f.\n",
                bugs, bug_rate);
    
        long universe_of_defects = 1L * 1024L * 1024L * 1024L;
        printf("The entire universe has %ld bugs.\n",
                universe_of_defects);
    
        double expected_bugs = bugs * bug_rate;
        printf("You are expected to have %f bugs.\n",
                expected_bugs);
    
        double part_of_universe = expected_bugs / universe_of_defects;
        printf("That is only a %e portion of the universe.\n",
                part_of_universe);
    
        // this makes no sense, just a demo of something weird
        char nul_byte = '\0';
        int care_percentage = bugs * nul_byte;
        printf("Which means you should care %d%%.\n",
                care_percentage);
    
        return 0;
    }
    

Here's what's going on in this little bit of nonsense:

ex7.c:1-4
    The usual start of a C program.
ex7.c:5-6
    Declare an int and double for some fake bug data.
ex7.c:8-9
    Print out those two, so nothing new here.
ex7.c:11
    Declare a huge number using a new type long for storing big numbers.
ex7.c:12-13
    Print out that number using %ld which adds a modifier to the usual %d. Adding 'l' (the letter ell) means "print this as a long decimal".
ex7.c:15-17
    Just more math and printing.
ex7.c:19-21
    Craft up a depiction of your bug rate compared to the bugs in the universe, which is a completely inaccurate calculation. It's so small though that we have to use %e to print it in scientific notation.
ex7.c:24
    Make a character, with a special syntax '\0' which creates a 'nul byte' character. This is effectively the number 0.
ex7.c:25
    Multiply bugs by this character, which produces 0 for how much you should care. This demonstrates an ugly hack you find sometimes.
ex7.c:26-27
    Print that out, and notice I've got a %% (two percent chars) so I can print a '%' (percent) character.
ex7.c:28-30
    The end of the main function.

This bit of source is entirely just an exercise, and demonstrates how some math works. At the end, it also demonstrates something you see in C, but not in many other languages. To C, a "character" is just an integer. It's a really small integer, but that's all it is. This means you can do math on them, and a lot of software does just that, for good or bad.

This last bit is your first glance at how C gives you direct access to the machine. We'll be exploring that more in later exercises.

# What You Should See

As usual, here's what you should see for the output:
    
    $ make ex7
    cc -Wall -g    ex7.c   -o ex7
    $ ./ex7
    You have 100 bugs at the imaginary rate of 1.200000.
    The entire universe has 1073741824 bugs.
    You are expected to have 120.000000 bugs.
    That is only a 1.117587e-07 portion of the universe.
    Which means you should care 0%.
    $
    

# How To Break It

Again, go through this and try breaking the printf by passing in the wrong arguments. See what happens when you try to print out that nul_byte variable too with %s vs. %c. When you break it, run it under Valgrind to see what it says about your breaking attempts.

# Extra Credit

   Make the number you assign to universe_of_defects various sizes until you get a warning from the compiler.
   What do these really huge numbers actually print out?
   Change long to unsigned long and try to find the number that makes that one too big.
   Go search online to find out what unsigned does.
   Try to explain to yourself (before I do in the next exercise) why you can multiply a char and an int.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















