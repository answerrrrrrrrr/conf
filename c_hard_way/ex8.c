Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 8: Sizes And Arrays

In the last exercise you did math, but with a '\0' (nul) character. This may be odd coming from other languages, since they try to treat "strings" and "byte arrays" as different beasts. C however treats strings as just arrays of bytes, and it's only the different printing functions that know there's a difference.

Before I can really explain the significance of this, I have to introduce a few more concepts: sizeof and arrays. Here's the code we'll be talking about:
    
    #include <stdio.h>
    
    int main(int argc, char *argv[])
    {
        int areas[] = {10, 12, 13, 14, 20};
        char name[] = "Zed";
        char full_name[] = {
            'Z', 'e', 'd',
             ' ', 'A', '.', ' ',
             'S', 'h', 'a', 'w', '\0'
        };
    
        // WARNING: On some systems you may have to change the
        // %ld in this code to a %u since it will use unsigned ints
        printf("The size of an int: %ld\n", sizeof(int));
        printf("The size of areas (int[]): %ld\n",
                sizeof(areas));
        printf("The number of ints in areas: %ld\n",
                sizeof(areas) / sizeof(int));
        printf("The first area is %d, the 2nd %d.\n",
                areas[0], areas[1]);
    
        printf("The size of a char: %ld\n", sizeof(char));
        printf("The size of name (char[]): %ld\n",
                sizeof(name));
        printf("The number of chars: %ld\n",
                sizeof(name) / sizeof(char));
    
        printf("The size of full_name (char[]): %ld\n",
                sizeof(full_name));
        printf("The number of chars: %ld\n",
                sizeof(full_name) / sizeof(char));
    
        printf("name=\"%s\" and full_name=\"%s\"\n",
                name, full_name);
    
        return 0;
    }
    

In this code we create a few arrays with different data types in them. Because arrays of data are so central to how C works, there's a huge number of ways to create them. For now, just use the syntax type name[] = {initializer}; and we'll explore more. What this syntax means is, "I want an array of type that is initialized to {..}." When C sees this it does the following:

   Look at the type, in this first case it's int.
   Look at the [] and see that there's no length given.
   Look at the initializer, {10, 12, 13, 14, 20} and figure out that you want those 5 ints in your array.
   Create a piece of memory in the computer, that can hold 5 integers one after another.
   Take the name you want, areas and assign it this location.

In the case of areas it's creating an array of 5 ints that contain those numbers. When it gets to char name[] = "Zed"; it's doing the same thing, except it's creating an array of 3 chars and assigning that to name. The final array we make is full_name, but we use the annoying syntax of spelling it out, one character at a time. To C, name and full_name are identical methods of creating a char array.

The rest of the file, we're using a keyword called sizeof to ask C how big things are in bytes. C is all about the size and location of pieces of memory and what you do with them. To help you keep that straight, it gives you sizeof so you can ask how big something is before you work with it.

This is where stuff gets tricky, so first let's run this and then explain further.

# What You Should See
    
    $ make ex8
    cc -Wall -g    ex8.c   -o ex8
    $ ./ex8
    The size of an int: 4
    The size of areas (int[]): 20
    The number of ints in areas: 5
    The first area is 10, the 2nd 12.
    The size of a char: 1
    The size of name (char[]): 4
    The number of chars: 4
    The size of full_name (char[]): 12
    The number of chars: 12
    name="Zed" and full_name="Zed A. Shaw"
    $
    

Now you see the output of these different printf calls and start to get a glimpse of what C is doing. Your output could actually be totally different from mine, since your computer might have different size integers. I'll go through my output:

5
    My computer thinks an int is 4 bytes in size. Your computer might use a different size if it's a 32-bit vs. 64-bit.
6
    The areas array has 5 integers in it, so it makes sense that my computer requires 20 bytes to store it.
7
    If we divide the size of areas by size of an int then we get 5 elements. Looking at the code, this matches what we put in the initializer.
8
    We then did an array access to get areas[0] and areas[1] which means C is "zero indexed" like Python and Ruby.
9-11
    We repeat this for the name array, but notice something odd about the size of the array? It says it's 4 bytes long, but we only typed "Zed" for 3 characters. Where's the 4th one coming from?
12-13
    We do the same thing with full_name and notice it gets this correct.
13
    Finally we just print out the name and full_name to prove that they actually are "strings" according to printf.

Make sure you can go through and see how these output lines match what was created. We'll be building on this and exploring more about arrays and storage next.

# How To Break It

Breaking this program is fairly easy. Try some of these:

   Get rid of the '\0' at the end of full_name and re-run it. Run it under Valgrind too. Now, move the definition of full_name to the top of main before areas. Try running it under Valgrind a few times and see if you get some new errors. In some cases, you might still get lucky and not catch any errors.
   Change it so that instead of areas[0] you try to print areas[10] and see what Valgrind thinks of that.
   Try other versions of these, doing it to name and full_name too.

# Extra Credit

   Try assigning to elements in the areas array with areas[0] = 100; and similar.
   Try assigning to elements of name and full_name.
   Try setting one element of areas to a character from name.
   Go search online for the different sizes used for integers on different CPUs.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















