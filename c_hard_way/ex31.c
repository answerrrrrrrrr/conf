Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 31: Debugging Code

I've already taught you about my awesome debug macros and you've been using them. When I debug code I use the debug() macro almost exclusively to analyze what's going on and track down the problem. In this exercise I'm going to teach you the basics of using gdb to inspect a simple program that runs and doesn't exit. You'll learn how to use gdb to attach to a running process, stop it, and see what's happening. After that I'll give you some little tips and tricks that you can use with gdb.

# Debug Printing Vs. GDB Vs. Valgrind

I approach debugging primarily with a "scientific method" style, where I come up with possible causes and then rule them out or prove they cause the defect. The problem many programmers have though is their panic and rush to solve a bug makes them feel like this approach will "slow them down". In their rush to solve they fail to notice that they're really just flailing around and gathering no useful information. I find that logging (debug printing) forces me to solve a bug scientifically and it's also just easier to gather information in more situations.

In addition to that, I also have these reasons for using debug printing as my primary debugging tool:

   You see an entire tracing of a program's execution with debug printing of variables which lets you track how things are going wrong. With gdb you have to place watch and debug statements all over for every thing you want and it's difficult to get a solid trace of the execution.
   The debug prints can stay in the code, and when you need them you can recompile and they come back. With gdb you have to configure the same information uniquely for every defect you have to hunt down.
   It's easier to turn on debug logging on a server that's not working right and then inspect the logs while it runs to see what's going on. System administrators know how to handle logging, they don't know how to use gdb.
   Printing things is just easier. Debuggers are always obtuse and weird with their own quirky interface and inconsistencies. There's nothing complicated about debug("Yo, dis right? %d", my_stuff);.
   Writing debug prints to find a defect forces you to actually analyze the code and use the scientific method. You can think of a debug usage as, "I hypothesize that the code is broken here." Then when you run it you get your hypothesis tested and if it's not broken then you can move to another part where it could be. This may seem like it takes longer, but it's actually faster because you go through a process of "differential diagnosis" and rule out possible causes until you find the real one.
   Debug printing works better with unit testing. You can actually just compile the debugs in all the time while you work, and when a unit test explodes just go look at the logs any time. With gdb you'd have to rerun the unit test under gdb and then trace through it to see what's going on.
   With valgrind you get the equivalent of debug prints for many memory related errors, so you don't need to use something like gdb to find those defects anymore.

Despite all these reasons that I rely on debug over gdb, I still use gdb in a few situations and I think you should have any tool that helps you get your work done. Sometimes, you just have to connect to a broken program and poke around. Or, maybe you've got a server that's crashing and you can only get at core files to see why. In these and a few other cases, gdb is the way to go, and it's always good to have as many tools as possible to help solve problems.

I then break down when I use gdb vs. valgrind vs. debug printing like this:

   Valgrind is used to catch all memory errors. I use gdb if valgrind is having problems or if using valgrind would slow the program down too much.
   Print with debug to diagnose and fix defects related to logic or usage. This amounts to about 90% of the defects after you start using Valgrind.
   Use gdb for the remaining "mystery weird stuff" or emergency situations to gather information. If Valgrind isn't turning anything up and I can't even print out the information I need, then I bust out gdb and start poking around. My use of gdb in this case is entirely to gather information. Once I have an idea of what's going on I go back to writing a unit test to cause the defect, and then do print statements to find out why.

# A Debugging Strategy

This process will actually work with any debugging technique you're going to use, whether that's Valgrind, debug printing, or using a debugger. I'm going to describe it in terms of using gdb since it seems people skip this process the most when using debuggers, but use this for every bug until you only need it on the very difficult ones.

   Start a little text file called notes.txt and use it as a kind of "lab notes" for ideas, bugs, problems, etc.
   Before you use gdb, write out the bug you're going to fix and what could be causing it.
   For each cause, write out the files and functions where you think the cause is coming from, or just write that you don't know.
   Now start gdb and pick the first possible cause with good file:function possibles and set breakpoints there.
   Use gdb to then run the program and confirm if that is the cause. The best way is to see if you can use the set command to either fix the program easily or cause the error immediately.
   If this isn't the cause, then mark in the notes.txt that it wasn't and why. Move on to the next possible cause that's easiest to debug, and keep adding information you gather.

In case you haven't noticed, this is basically the scientific method. You write down a set of hypotheses, then you use debugging to prove or disprove them. This gives you insight into more possible causes and then eventually you find it. This process helps you avoid going over the same possible causes repeatedly even though you've found they aren't possible.

You can also do this with debug printing, the only difference is you actually write out your hypotheses in the source code where you think the problem is instead of the notes.txt. In a way, debug printing forces you to tackle bugs scientifically since you have to write out hypotheses as print statements.

# Using GDB

The program I'll debug in this exercise is just a while-loop that doesn't terminate correctly. I'm putting a small usleep call in it so that there's something interesting to troll through as well.
    
    #include <unistd.h>
    
    int main(int argc, char *argv[])
    {
        int i = 0;
    
        while(i < 100) {
            usleep(3000);
        }
    
        return 0;
    }
    

Compile this like normal and then start it under gdb like this: gdb ./ex31

Once it's running I want you to play around with these gdb commands to see what they do and how to use them.

help COMMAND
    Get a short help with COMMAND.
break file.c:(line|function)
    Sets a break point where you want to pause execution. You can give lines or function names to break at after the file.
run ARGS
    Runs the program, using the ARGS as arguments to the program.
cont
    Continues execution until a new breakpoint or error.
step
    

Step through the code, but move into functions. Use this to

trace into a function and see what it's doing.

next
    Just like step, but go over functions by just running them.
backtrace (or bt)
    Does a "backtrace", which dumps the trace of function calls leading to the current point in the program. Very useful for figuring out how you got there, since it also prints the parameters that were passed to each function. It's also similar to what Valgrind reports when you have a memory error.
set var X = Y
    Set variable X equal to Y.
print X
    Prints out the value of X, and you can usually use C syntax to access the values of pointers and contents of structs.
ENTER
    The ENTER key just repeats the last command.
quit
    Exits gdb

Those are the majority of commands I use with gdb. Your job is to now play with these and ex31 so you can get familiar with the output.

Once you're familiar with gdb you'll want to play with it some more. Try using it on more complicated programs like devpkg to see if you can alter the program's execution or analyze what it's doing.

# Process Attaching

The most useful thing about gdb is the ability to attach to a running program and debug it right there. When you have a crashing server or a GUI program, you can't usually start it under gdb like you just did. Instead, you have to start it, hope it doesn't crash right away, then attach to it and set a breakpoint. In this part of the exercise I'll show you how to do that.

After you exit gdb I want you to restart ex31 if you stopped it, and then start another Terminal window so you can process attach to it. Process attaching is where you tell gdb to connect to a program that's already running so you can inspect it live. It stops the program and then you can walk through it, and when you're done it'll continue just like normal.

Here's a session of me doing it to ex31, stepping through it, then fixing the while-loop to make it exit.
    
    $ ps ax | grep ex31
    10026 s000  S+     0:00.11 ./ex31
    10036 s001  R+     0:00.00 grep ex31
    
    $ gdb ./ex31 10026
    GNU gdb 6.3.50-20050815 (Apple version gdb-1705) (Fri Jul  1 10:50:06 UTC 2011)
    Copyright 2004 Free Software Foundation, Inc.
    GDB is free software, covered by the GNU General Public License, and you are
    welcome to change it and/or distribute copies of it under certain conditions.
    Type "show copying" to see the conditions.
    There is absolutely no warranty for GDB.  Type "show warranty" for details.
    This GDB was configured as "x86_64-apple-darwin"...Reading symbols for shared libraries .. done
    
    /Users/zedshaw/projects/books/learn-c-the-hard-way/code/10026: No such file or directory
    Attaching to program: `/Users/zedshaw/projects/books/learn-c-the-hard-way/code/ex31', process 10026.
    Reading symbols for shared libraries + done
    Reading symbols for shared libraries ++........................ done
    Reading symbols for shared libraries + done
    0x00007fff862c9e42 in __semwait_signal ()
    
    (gdb) break 8
    Breakpoint 1 at 0x107babf14: file ex31.c, line 8.
    
    (gdb) break ex31.c:11
    Breakpoint 2 at 0x107babf1c: file ex31.c, line 12.
    
    (gdb) cont
    Continuing.
    
    Breakpoint 1, main (argc=1, argv=0x7fff677aabd8) at ex31.c:8
    8      while(i < 100) {
    
    (gdb) p i
    $1 = 0
    
    (gdb) cont
    Continuing.
    
    Breakpoint 1, main (argc=1, argv=0x7fff677aabd8) at ex31.c:8
    8      while(i < 100) {
    
    (gdb) p i
    $2 = 0
    
    (gdb) list
    3  
    4  int main(int argc, char *argv[])
    5  {
    6      int i = 0;
    7  
    8      while(i < 100) {
    9          usleep(3000);
    10     }
    11 
    12     return 0;
    
    (gdb) set var i = 200
    
    (gdb) p i
    $3 = 200
    
    (gdb) next
    
    Breakpoint 2, main (argc=1, argv=0x7fff677aabd8) at ex31.c:12
    12     return 0;
    
    (gdb) cont
    Continuing.
    
    Program exited normally.
    (gdb) quit
    $
    

Note

On OSX you may see a GUI prompt for the root password, and even after you give it you still get an error from gdb saying "Unable to access task for process-id XXX: (os/kern) failure." In that case stop both gdb and the ex31 program, then start over and it should work as long as you successfully entered the root password.

I'll walk through this session and explain what I did:

gdb:1
    I use ps to find out what the process id is of the ex31 I want to attach.
gdb:5
    I'm attaching using gdb ./ex31 PID replacing PID with the process id I have.
gdb:6-19
    gdb prints out a bunch of information about it's license and then all the things it's reading.
gdb:21
    The program is attached and stopped at this point, so now I set a breakpoint at line 8 in the file with break. I'm assuming that I'm already in the file I want to break when I do this.
gdb:24
    A better way to do a break, is give file.c:line format so you can be sure you did the right location. I do that in this break.
gdb:27
    I use cont to continue processing until I hit a breakpoint.
gdb:30-31
    The breakpoint is reached so gdb prints out variables I need to know about (argc and argv) and where it's stopped, then the line of code for the breakpoint.
gdb:33-34
    I use the abbreviation for print "p" to print out the value of the i variable. It's 0.
gdb:36
    Continue again to see if i changes.
gdb:42
    Print out i again, and nope it's not changing.
gdb:45-55
    Use list to see what the code is, and then I realize it's not exiting because I'm not incrementing i.
gdb:57
    Confirm my hypothesis that i needs to change by using the set command to change it to be i = 200. This is one of the best features of gdb as it lets you "fix" a program really quick to see if you're right.
gdb:59
    Print out i just to make sure it changed.
gdb:62
    Use next to move to the next piece of code, and I see that the breakpoint at ex31.c:12 is hit, so that means the while-loop exited. My hypothesis is correct, I need to make i change.
gdb:67
    Use cont to continue and the program exits like normal.
gdb:71
    I finally use quit to get out of gdb.

# GDB Tricks

Here's a list of simple tricks you can do with GDB:

gdb --args
    Normally gdb takes arguments you give it and assumes they are for itself. Using --args passes them to the program.
thread apply all bt
    Dumps a backtrace for all threads. Very useful.
gdb --batch --ex r --ex bt --ex q --args
    Runs the program so that, if it bombs you get a backtrace.

?
    Got one? Leave it in the comments.

# Extra Credit

   Find a graphical debugger and compare using it to raw gdb. These are useful when the program you're looking at is local, but they are pointless if you have to debug a program on a server.
   You can enable "core dumps" on your OS, and when a program crashes you'll get a core file. This core file is like a post-mortem of the program so you can load up what happened right at the crash and see what caused it. Change ex31.c so that it crashes after a few iterations, then try to get a core dump and analyze it.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 


















