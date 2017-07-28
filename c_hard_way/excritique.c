Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Deconstructing K&RC

System Message: WARNING/2 (krcritique.rst, line 3)

Title underline too short.
    
    
    Deconstructing ``K&RC``
    ===================
    

When I was a kid I read this awesome book called "The C Programming Language" by the language's creators, Brian Kernighan and Dennis Ritchie. This book taught me and many people of my generation, and a generation before, how to write C code. You talk to anyone, whether they know C or not, and they'll say, "You can't beat K&RC. It's the best C book." It is an established piece of programmer lore that is not soon to die.

I myself believed that until I started writing this book. You see, K&RC is actually riddled with bugs and bad style. Its age is no excuse. These were bugs when they wrote the first printing, and the 42nd printing. I hadn't actually realized just how bad most of the code was in this book and recommended it to many people. After reading through it for just an hour I decided that it needs to be taken down from its pedestal and relegated to history rather than vaunted as state of the art.

I believe it is time to lay this book to rest, but I want to use it as an exercise for you in finding hacks, attacks, defects, and bugs by going through K&RC to break all the code. That's right, you are going to destroy this sacred cow for me, and you're going to have no problem doing it. When you are done doing this, you will have a finely honed eye for defect. You will also have an informed opinion of the book's actual quality, and will be able to make your own decisions on how to use the knowledge it contains.

In this chapter we will use all the knowledge you've gained from this book, and spend it reviewing the code in K&RC. What we will do is take many pieces of code from the book, find all the bugs in it, and write a unit test that exercises the bugs. We'll then run this test under Valgrind to get statistics and data, and then we'll fix the bugs with a redesign.

This will obviously be a long chapter so I'm going to only do a handful of these and then I'm going have you do the rest. I'll provide a guide that is each page, with the code on it, and hints to the bugs that it has. Your job is to then tear that piece of code apart and try to think like an attacker trying to break the code.

Note

As you read this, if you feel that I am being disrespectful to the authors, then that's not my intent. I respect the authors more than anything you know and owe them a debt of gratitude for writing their book. My criticisms here are both for educational purposes of teaching people modern C code, and to destroy the belief in their work as a item of worship that cannot be questioned.

However, if when you read this you have feelings of me insulting you then just stop reading. You will gain nothing from this chapter but personal grief because you've attached your identity to K&RC and my criticisms will only be taken personally.

# An Overall Critique Of Correctness

The primary problem K&RC has is its view of "correctness" comes from the first system it was used on: Unix. In the world of Unix software programs have a particular set of properties:

   Programs are started and then exit, making resource allocation easier.
   Most functions are only called by other parts of the same program in set ways.
   The inputs to the program are limited to "expert" restricted users.

In the context of this 1970's computing style, K&RC is actually correct. As long as only trusted people run complete cohesive programs that exit and clean up all their resources then their code is fine.

Where K&RC runs into problems is when the functions or code snippets are taken out of the book and used in other programs. Once you take many of these code snippets and try use them in some other program they fall apart. They then have blatant buffer overflows, bugs, and problems that a beginner will trip over.

Another problem is that software these days doesn't exit right away, but instead it stays running for long periods of time because they're servers, desktop applications and mobile applications. The old style of "leaving the cleanup to the OS" doesn't work in the modern world the way it did back in the day.

The final problem though is that no software lives in a vacuum anymore. Software is now frequently attacked by people over network connections in an attempt to gain special privilege or simple street cred. The idea that "nobody will ever do that" is dead, and actually that's probably the first thing somebody will do.

The best way to summarize the problem of K&RC "correctness" is with an example from English. Imagine if you have the pair of sentences, "Jack and Jill went up the hill. He fell down." Well, from context clues you know that "He" means Jack. However, if you have that sentence on its own it's not clear who "He" is. Now, if you put that sentence at the end of another sentence you can get an unclear pronoun reference: "Jack and Frank went up the hill. He fell down." Which "He" are we talking about in that sentence?

This is how the code in K&RC works. As long as that code is not used in other programs without serious analysis of the entire software then it works. The second you take many of the functions out and try to use them in other systems they fall apart. And, what's the point of a book full of code you can't actually use in your own programs?

## A First Demonstration Defect

The following copy function is found in the very first chapter and is an example of copying two strings. Here's a new source file to demonstrate the defects in this function.
    
    #include <stdio.h>
    #include <assert.h>
    #include <stdlib.h>
    
    #define MAXLINE 10 // in the book this is 1000
    
    void copy(char to[], char from[])
    {
        int i;
    
        i = 0;
        while((to[i] = from[i]) != '\0')
            ++i;
    }
    
    int main(int argc, char *argv[])
    {
        int i;
    
        // use heap memory as many modern systems do
        char *line = malloc(MAXLINE);
        char *longest = malloc(MAXLINE);
    
        assert(line != NULL && longest != NULL && "memory error");
    
        // initialize it but make a classic "off by one" error
        for(i = 0; i < MAXLINE; i++) {
            line[i] = 'a';
        }
    
        // cause the defect
        copy(longest, line);
    
        free(line);
        free(longest);
    
        return 0;
    }
    

In the above example, I'm doing something that is fairly common: switching from using stack allocation to heap allocation with malloc. What happens is, typically malloc returns memory from the heap, and so the bytes after it are not initialized. Then you see me use a loop to accidentally initialize it wrong. This is a common defect, and one of the reasons we avoided classic style C strings in this book. You could also have this bug in programs that read from files, sockets, or other external resources. It is a very common bug, probably the most common in the world.

Before the switch to heap memory, this program probably ran just fine because the stack allocated memory will probably have a '\0' character at the end on accident. In fact, it would appear to run fine almost always since it just runs and exits quickly.

What's the effect of running this new program with copy used wrong?
    
    $ make 1.9-1
    cc     1.9-1.c   -o 1.9-1
    $ ./1.9-1
    $
    $ valgrind ./1.9-1
    ==2162== Memcheck, a memory error detector
    ==2162== Copyright (C) 2002-2010, and GNU GPL'd, by Julian Seward et al.
    ==2162== Using Valgrind-3.6.0.SVN-Debian and LibVEX; rerun with -h for copyright info
    ==2162== Command: ./1.9-1
    ==2162== 
    ==2162== Invalid read of size 1
    ==2162==    at 0x4005C0: copy (in /home/zedshaw/projects/books/learn-c-the-hard-way/code/krc/1.9-1)
    ==2162==    by 0x400651: main (in /home/zedshaw/projects/books/learn-c-the-hard-way/code/krc/1.9-1)
    ==2162==  Address 0x51b104a is 0 bytes after a block of size 10 alloc'd
    ==2162==    at 0x4C2815C: malloc (vg_replace_malloc.c:236)
    ==2162==    by 0x4005E6: main (in /home/zedshaw/projects/books/learn-c-the-hard-way/code/krc/1.9-1)
    ==2162== 
    ==2162== Invalid write of size 1
    ==2162==    at 0x4005C3: copy (in /home/zedshaw/projects/books/learn-c-the-hard-way/code/krc/1.9-1)
    ==2162==    by 0x400651: main (in /home/zedshaw/projects/books/learn-c-the-hard-way/code/krc/1.9-1)
    ==2162==  Address 0x51b109a is 0 bytes after a block of size 10 alloc'd
    ==2162==    at 0x4C2815C: malloc (vg_replace_malloc.c:236)
    ==2162==    by 0x4005F4: main (in /home/zedshaw/projects/books/learn-c-the-hard-way/code/krc/1.9-1)
    ==2162== 
    ==2162== Invalid read of size 1
    ==2162==    at 0x4005C5: copy (in /home/zedshaw/projects/books/learn-c-the-hard-way/code/krc/1.9-1)
    ==2162==    by 0x400651: main (in /home/zedshaw/projects/books/learn-c-the-hard-way/code/krc/1.9-1)
    ==2162==  Address 0x51b109a is 0 bytes after a block of size 10 alloc'd
    ==2162==    at 0x4C2815C: malloc (vg_replace_malloc.c:236)
    ==2162==    by 0x4005F4: main (in /home/zedshaw/projects/books/learn-c-the-hard-way/code/krc/1.9-1)
    ==2162== 
    ==2162== 
    ==2162== HEAP SUMMARY:
    ==2162==     in use at exit: 0 bytes in 0 blocks
    ==2162==   total heap usage: 2 allocs, 2 frees, 20 bytes allocated
    ==2162== 
    ==2162== All heap blocks were freed -- no leaks are possible
    ==2162== 
    ==2162== For counts of detected and suppressed errors, rerun with: -v
    ==2162== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 4 from 4)
    $
    

As you've already learned, Valgrind will show you all of your sins in full color. In this case, a perfectly harmless seeming program has a ton of "Invalid read of size 1". If you kept running it you'd find other errors pop up at random.

Now, in the context of the entire program in the original K&RC example, this function will work correctly. However, the second this function is called with longest and line uninitialized, initialized wrong, without a trailing '\0' character, then you'll hit difficult to debug errors.

This is the failing of the book. While the code works in the book, it does not work in many other situations leading to difficult to spot defects, and those are the worst kind of defects for a beginner (or expert). Instead of code that only works in this delicate balance, we will strive to create code that has a higher probability of working in any situation.

## Why copy() Fails

Many people have looked at this copy function and thought that it is not defective. They claim that, as long as it's used correctly, it is correct. One person even went so far as to say, "It's not defective, it's just unsafe." Odd, since I'm sure this person wouldn't get into a car if the manufacturer said, "Our car is not defective, it's just unsafe."

However, there is a way to formally prove that this function is defective by enumerating the possible inputs and then seeing if any of them cause the while loop to never terminate.

What we'll do is have two strings, A and B, and figure out what copy() does with them:

   A = {'a','b','\0'}; B = {'a', 'b', '\0'};  copy(A,B);
   A = {'a','b'}; B = {'a', 'b', '\0'};  copy(A,B);
   A = {'a','b','\0'}; B = {'a', 'b'};  copy(A,B);
   A = {'a','b'}; B = {'a', 'b'};  copy(A,B);

This is all the basic permutations of strings that can be passed to the function based on whether they are terminated with a '\0' or not. To be complete I'm covering all possible permutations, even if they seem irrelevant. You may think there's no need to include permutations on A, but as you'll see in the analysis, not including A fails to find buffer overflows that are possible.

We can then go through each case and determine if the while loop in copy() terminates:

   while-loop finds '\0' in B, copy fits in A, terminates.
   while-loop finds '\0' in B, overflows A, terminates.
   while-loop does not find '\0' in B, overflows A, does not terminate.
   while-loop does not find '\0' in B, overflows A, does not terminate.

This provides a formal proof that the function is defective because there are possible inputs that causes the while-loop to run forever or overflow the target. If you were to try and use this function safely, you would need to follow all paths to its usage, and confirm that the data is correct along every path. That gives every path to this function a 50% to 75% chance it will fail with just the inputs above. You could find some more permutations of failure but these are the most basic ones.

Let's now compare this to a copy function that knows the lengths of all the inputs to see what it's probability of failure is:

   A = {'a','b','\0'}; B = {'a', 'b', '\0'};  safercopy(2, A, 2, B);
   A = {'a','b'}; B = {'a', 'b', '\0'};  safercopy(2, A, 2, B);
   A = {'a','b','\0'}; B = {'a', 'b'};  safercopy(2, A, 2, B);
   A = {'a','b'}; B = {'a', 'b'};  safercopy(2, A, 2, B);

Also assume that the safercopy() function uses a for-loop that does not test for a '\0' only, but instead uses the given lengths to determine the amount to copy. With that we can then do the same analysis:

   for-loop processes 2 characters of A, terminates.
   for-loop processes 2 characters of A, terminates.
   for-loop processes 2 characters of A, terminates.
   for-loop processes 2 characters of A, terminates.

In every case the for-loop variant with string length given as arguments will terminate no matter what. To really test the for-loop variant we'd need to add some permutations for differing lengths of strings A and B, but in every case the for-loop will always stop because it will only go through a fixed previously known finite number of characters.

That means the for-loop will never loop forever, and as long as it handles all the possible differing lengths of A and B, never overflow either side. The only way to break safercopy() is to lie about the lengths of the strings, but even then it will still always terminate. The worst possible scenario for the safercopy() function is that you are given an erroneous length for one of the strings and that string does not have a '\0' properly, so the function buffer overflows.

This shows exactly why the copy() function is defective, because it does not terminate cleanly for most possible inputs, and is only reliable for one of the conditions: B terminated and A the right size. It also shows why a for-loop variant with a given fixed length for each input is superior.

Finally, the significance of this is that I've effectively done a formal proof (well, mostly formal) that shows what you should be doing to analyze code. Each function has to stand on its own and not have any defects such as while-loops that do not terminate. In the above discussion I've shown that the original K&RC is defective, and fatally so since there is no way to fix it given the inputs. There's no way from just a pointer to ask if a string is properly formed since the only way to test that is to scan it, and scanning it runs into this same problem.

## But, That's Not A C String

Some folks then defend this function (despite the proof above) by claiming that the strings in the proof aren't C strings. They want to apply an artful dodge that says "the function is not defective because you aren't giving it the right inputs", but I'm saying the function is defective because most of the possible inputs cause it to crash the software.

The problem with this mindset is there's no way to confirm that a C string is valid. Imagine you wanted to write a little assert_good_string function that checks if a C string is correctly terminated before using it. This function needs to go to the end of the string and see if there's a '\0' terminator. How does it do this? This function would also have to scan the target function to confirm that it ended in '\0', which means it has the same problem as copy() because the input may not be terminated.

This may seem silly, but people actually do this with strlen(). They take an input and think that they just have to run strlen() on the input to confirm that it's the right length, but strlen() itself has the same fatal flaw because it has to scan and if the string isn't terminated it will also overflow.

This means any attempt to fix the problem using just C strings also has this problem. The only way to solve it is to include the length of every string and use that to scan it.

If you can't validate a C string in your function, then your only choice is to do full code reviews manually. This introduces human error and no matter what you do the error will happen.

## Just Don't Do That

Another argument in favor of this copy() function is when the proponents of K&RC state that you are "just supposed to not use bad strings". Despite the mountains of empirical evidence that this is impossible in C code, they are basically correct and that's what I'm teaching in this exercise. But, instead of saying "just don't do that by checking all possible inputs", I'm advocating "just don't do that by not using this kind of function". I'll explain further.

In order to confirm that all inputs to this function are valid I have to go through a code review process that involves this:

   Find all the places the copy() function is called.
   Trace backwards from that call point to where the inputs are created.
   Confirm that the data is created correctly.
   Follow the path from the creation point of the data to where it's used and confirm that no line of code alters the data.
   Repeat this for all paths and all branches, including all loops and if-statements involving the data.

In my experience this is only possible in small programs like the little ones that K&RC has. In real software the number of possible branches you'd need to check is much too high for most people to validate, especially in a team environment where individuals have varying degrees of capability. A way to quantify this difficulty is that each branch in the code leading to a function like copy() has a 50-70% chance of causing the defect.

However, if you can use a different function and avoid all of these checks then doesn't that mean the copy() function is defective by comparison? These people are right, the solution is to "just not do that" by just not using the copy() function. You can change the function to one that includes the sizes of the two strings and the problem is solved. If that's the case then the people who think "just don't do that" have just proved that the function is defective, because the simpler way to "not do that" is to use a better function.

If you think copy() is valid as long as you avoid the errors I outline, and if safercopy() avoids the errors, then safercopy() is superior and copy() is defective by comparison.

## Stylistic Issues

A more minor critique of the book is that the style is not only old, but just error prone and annoyingly "clever". Take the code you just saw again and look at the while-loop in copy. There's no reason to write this loop this way, as the compiler can just as easily work with a for-loop and without the clever triple-equality trick. The original code also has a while-loop without braces, but an if-statement with braces, which leads to even more confusion:
    
    /* bad use of while loop with compound if-statement */
    while ((len = getline(line, MAXLINE)) > 0)
        if (len > max) {
            max = len;
            copy(longest, line);
        }
    if (max > 0) /* there was a line */
        printf("%s", longest);
    

This code is incredibly error prone because you can't easily tell where the pair of if-statements and the while-loop are paired. A quick glance makes it seem like this while-loop will loop both if-statements, but it doesn't. In modern C code you would instead just use braces all the time and avoid the confusion completely.

While the book could be forgiven for this because of its age, it has been republished in this form 42 times, and it was updated for the ANSI standard. At some point in its history you'd think the authors or some publisher ghostwriter could have been bothered to update the book's style. However, this is the problem with sacred cows. Once they become idols of worship people are reluctant to question them or modify them.

In the rest of this chapter though we will be modernizing the code in K&RC to fit the style you've been learning throughout this book. It will be more verbose, but it will be clearer and less error prone because of this slight increase in verbosity.

# Chapter 1 Examples

Now we begin...

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















