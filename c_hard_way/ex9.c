Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 9: Arrays And Strings

In the last exercise you went through an introduction to creating basic arrays and how they map to strings. In this exercise we'll more completely show the similarity between arrays and strings, and get into more about memory layouts.

This exercise shows you that C stores its strings simply as an array of bytes, terminated with the '\0' (nul) byte. You probably clued into this in the last exercise since we did it manually. Here's how we do it in another way to make it even more clear by comparing it to an array of numbers:
    
    #include <stdio.h>
    
    int main(int argc, char *argv[])
    {
        int numbers[4] = {0};
        char name[4] = {'a'};
    
        // first, print them out raw
        printf("numbers: %d %d %d %d\n",
                numbers[0], numbers[1],
                numbers[2], numbers[3]);
    
        printf("name each: %c %c %c %c\n",
                name[0], name[1],
                name[2], name[3]);
    
        printf("name: %s\n", name);
    
        // setup the numbers
        numbers[0] = 1;
        numbers[1] = 2;
        numbers[2] = 3;
        numbers[3] = 4;
    
        // setup the name
        name[0] = 'Z';
        name[1] = 'e';
        name[2] = 'd';
        name[3] = '\0';
    
        // then print them out initialized
        printf("numbers: %d %d %d %d\n",
                numbers[0], numbers[1],
                numbers[2], numbers[3]);
    
        printf("name each: %c %c %c %c\n",
                name[0], name[1],
                name[2], name[3]);
    
        // print the name like a string
        printf("name: %s\n", name);
    
        // another way to use name
        char *another = "Zed";
    
        printf("another: %s\n", another);
    
        printf("another each: %c %c %c %c\n",
                another[0], another[1],
                another[2], another[3]);
    
        return 0;
    }
    

In this code, we setup some arrays the tedious way, by assigning a value to each element. In numbers we are setting up numbers, but in name we're actually building a string manually.

# What You Should See

When you run this code you should see first the arrays printed with their contents initialized to zero, then in its initialized form:
    
    $ make ex9
    cc -Wall -g    ex9.c   -o ex9
    $ ./ex9
    numbers: 0 0 0 0
    name each: a   
    name: a
    numbers: 1 2 3 4
    name each: Z e d 
    name: Zed
    another: Zed
    another each: Z e d
    $
    

You'll notice some interesting things about this program:

   I didn't have to give all 4 elements of the arrays to initialize them. This is a short-cut that C has where, if you set just one element, it'll fill the rest in with 0.
   When each element of numbers is printed they all come out as 0.
   When each element of name is printed, only the first element 'a' shows up because the '\0' character is special and won't display.
   Then the first time we print name it only prints "a" because, since the array will be filled with 0 after the first 'a' in the initializer, then the string is correctly terminated by a '\0' character.
   We then setup the arrays with a tedious manual assignment to each thing and print them out again. Look at how they changed. Now the numbers are set, but see how the name string prints my name correctly?
   There's also two syntaxes for doing a string: char name[4] = {'a'} on line 6 vs. char *another = "name" on line 44. The first one is less common and the second is what you should use for string literals like this.

Notice that I'm using the same syntax and style of code to interact with both an array of integers and an array of characters, but that printf thinks that the name is just a string. Again, this is because to the C language there's no difference between a string and an array of characters.

Finally, when you make string literals you should usually use the char *another = "Literal" syntax. This works out to be the same thing, but it's more idiomatic and easier to write.

# How To Break It

The source of almost all bugs in C come from forgetting to have enough space, or forgetting to put a '\0' at the end of a string. In fact it's so common and hard to get right that the majority of good C code just doesn't use C style strings. In later exercises we'll actually learn how to avoid C strings completely.

In this program the key to breaking it is to forget to put the '\0' character at the end of the strings. There's a few ways to do this:

   Get rid of the initializers that setup name.
   Accidentally set name[3] = 'A'; so that there's no terminator.
   Set the initializer to {'a','a','a','a'} so there's too many 'a' characters and no space for the '\0' terminator.

Try to come up with some other ways to break this, and as usual run all of these under Valgrind so you can see exactly what is going on and what the errors are called. Sometimes you'll make these mistakes and even Valgrind can't find them, but try moving where you declare the variables to see if you get the error. This is part of the voodoo of C, that sometimes just where the variable is located changes the bug.

# Extra Credit

   Assign the characters into numbers and then use printf to print them a character at a time. What kind of compiler warnings did you get?
   Do the inverse for name, trying to treat it like an array of int and print it out one int at a time. What does Valgrind think of that?
   How many other ways can you print this out?
   If an array of characters is 4 bytes long, and an integer is 4 bytes long, then can you treat the whole name array like it's just an integer? How might you accomplish this crazy hack?
   Take out a piece of paper and draw out each of these arrays as a row of boxes. Then do the operations you just did on paper to see if you get them right.
   Convert name to be in the style of another and see if the code keeps working.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















