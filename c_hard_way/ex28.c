Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 28: Intermediate Makefiles

In the next three Exercises you'll create a skeleton project directory to use in building your C programs later. This skeleton directory will be used in the rest of the book, and in this exercise I'll cover just the Makefile so you can understand it.

The purpose of this structure is to make it easy to build medium sized programs without having to resort to configure tools. If done right you can get very far with just GNU make and some small shell scripts.

# The Basic Project Structure

The first thing to do is make a c-skeleton directory and then put a set of basic files and directories in it that many projects have. Here's my starter:
    
    $ mkdir c-skeleton
    $ cd c-skeleton/
    $ touch LICENSE README.md Makefile
    $ mkdir bin src tests
    $ cp dbg.h src/   # this is from Ex20
    $ ls -l
    total 8
    -rw-r--r--  1 zedshaw  staff     0 Mar 31 16:38 LICENSE
    -rw-r--r--  1 zedshaw  staff  1168 Apr  1 17:00 Makefile
    -rw-r--r--  1 zedshaw  staff     0 Mar 31 16:38 README.md
    drwxr-xr-x  2 zedshaw  staff    68 Mar 31 16:38 bin
    drwxr-xr-x  2 zedshaw  staff    68 Apr  1 10:07 build
    drwxr-xr-x  3 zedshaw  staff   102 Apr  3 16:28 src
    drwxr-xr-x  2 zedshaw  staff    68 Mar 31 16:38 tests
    $ ls -l src
    total 8
    -rw-r--r--  1 zedshaw  staff  982 Apr  3 16:28 dbg.h
    $
    

At the end you see me do an ls -l so you can see the final results.

Here's what each of these does:

LICENSE
    If you release the source of your projects you'll want to include a license. If you don't though, the code is copyright by you and nobody has rights to it by default.
README.md
    Basic instructions for using your project go here. It ends in .md so that it will be interpreted as markdown.
Makefile
    The main build file for the project.
bin/
    Where programs that users can run go. This is usually empty and the Makefile will create it if it's not there.
build/
    Where libraries and other build artifacts go. Also empty and the Makefile will create it if it's not there.
src/
    Where the source code goes, usually .c and .h files.
tests/
    Where automated tests go.
src/dbg.h
    I copied the dbg.h from Exercise 20 into src/ for later.

I'll now break down each of the components of this skeleton project so you can understand how it works.

# Makefile

The first thing I'll cover is the Makefile because from that you can understand how everything else works. The Makefile in this exercise is much more detailed than ones you've used so far, so I'm going to break it down after you type it in:
    
    CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
    LIBS=-ldl $(OPTLIBS)
    PREFIX?=/usr/local
    
    SOURCES=$(wildcard src/**/*.c src/*.c)
    OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
    
    TEST_SRC=$(wildcard tests/*_tests.c)
    TESTS=$(patsubst %.c,%,$(TEST_SRC))
    
    TARGET=build/libYOUR_LIBRARY.a
    SO_TARGET=$(patsubst %.a,%.so,$(TARGET))
    
    # The Target Build
    all: $(TARGET) $(SO_TARGET) tests
    
    dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
    dev: all
    
    $(TARGET): CFLAGS += -fPIC
    $(TARGET): build $(OBJECTS)
           ar rcs $@ $(OBJECTS)
           ranlib $@
    
    $(SO_TARGET): $(TARGET) $(OBJECTS)
           $(CC) -shared -o $@ $(OBJECTS)
    
    build:
           @mkdir -p build
           @mkdir -p bin
    
    # The Unit Tests
    .PHONY: tests
    tests: CFLAGS += $(TARGET)
    tests: $(TESTS)
           sh ./tests/runtests.sh
    
    valgrind:
           VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)
    
    # The Cleaner
    clean:
           rm -rf build $(OBJECTS) $(TESTS)
           rm -f tests/tests.log
           find . -name "*.gc*" -exec rm {} \;
           rm -rf `find . -name "*.dSYM" -print`
    
    # The Install
    install: all
           install -d $(DESTDIR)/$(PREFIX)/lib/
           install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/
    
    # The Checker
    BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
    check:
           @echo Files with potentially dangerous functions.
           @egrep $(BADFUNCS) $(SOURCES) || true
    

Remember that you need to indent the Makefile consistently with tab characters. Your editor should know that and do the right thing, but if it doesn't then get a different text editor. No programmer should use an editor that fails at something so simple.

# The Header

This makefile is designed to build a library we'll be working on later and to do so reliably on almost any platform by using special features of GNU make. I'll break down each part in sections, starting with the header.

Makefile:1
    These are the usual CFLAGS that you set in all of your projects, but with a few others that may be needed to build libraries. You may need to adjust these for different platforms. Notice the OPTFLAGS variable at the end which lets people augment the build options as needed.
Makefile:2
    Options used when linking a library, and allows someone else to augment the linking options using the OPTLIBS variable.
Makefile:3
    Setting an optional variable called PREFIX that will only have this value if the person running the Makefile didn't already give a PREFIX setting. That's what the ?= does.
Makefile:5
    This fancy line of awesome dynamically creates the SOURCES variable by doing a wildcard search for all *.c files in the src/ directory. You have to give both src/**/*.c and src/*.c so that GNU make will include the files in src and also the ones below it.
Makefile:6
    Once you have the list of source files, you can then use the patsubst to take the SOURCES list of *.c files and make a new list of all the object files. You do this by telling patsubst to change all %.c extensions to %.o and then those are assigned to OBJECTS.
Makefile:8
    Using the wildcard again to find all the test source files for the unit tests. These are separate from the library's source files.
Makefile:9
    Then using the same patsubst trick to dynamically get all the TEST targets. In this case I'm stripping away the .c extension so that a full program will be made with the same name. Previously I had replaced the .c with {.o} so an object file is created.
Makefile:11
    Finally, we say the ultimate target is build/libYOUR_LIBRARY.a, which you will change to be whatever library you are actually trying to build.

This completes the top of the Makefile, but I should explain what I mean by "lets people augment the build". When you run make you can do this:
    
    
    # WARNING! Just a demonstration, won't really work right now.
    # WARNING! Just a demonstration, won't really work right now.
    # this installs the library into /tmp
    $ make PREFIX=/tmp install
    # this tells it to add pthreads
    $ make OPTFLAGS=-pthread
    

If you pass in options that match the same kind of variables you have in your Makefile, then those will show up in your build. You can then use this to change how the Makefile runs. The first one alters the PREFIX so that it installs into /tmp instead. The second one sets OPTFLAGS so that the -pthread option is present.

## The Target Build

Continuing with the breakdown of the Makefile I have actually building the object files and targets:

Makefile:14
    Remember that the first target is what make will run by default when no target is given. In this case it's called all: and it gives $(TARGET) tests as the targets to build. Look up at the TARGET variable and you see that's the library, so all: will first build the library. The tests target is then further down in the Makefile and builds the unit tests.
Makefile:16
    Another target for making "developer builds" that introduces a technique for changing options for just one target. If I do a "dev build" I want the CFLAGS to include options like -Wextra that are useful for finding bugs. If you place them on the target line as options like this, then give another line that says the original target (in this case all) then it will change the options you set. I use this for setting different flags on different platforms that need it.
Makefile:19
    Builds the TARGET library, whatever that is, and also uses the same trick from line 15 of giving a target with just options changes to alter them for this run. In this case I'm adding -fPIC just for the library build using the += syntax to add it on.
Makefile:20
    Now the real target where I say first make the build directory, then compile all the OBJECTS.
Makefile:21
    Runs the ar command which actually makes the TARGET. The syntax $@ $(OBJECTS) is a way of saying, "put the target for this Makefile source here and all the OBJECTS after that". In this case the $@ maps back to the $(TARGET) on line 19, which maps to build/libYOUR_LIBRARY.a. It seems like a lot to keep track of this indirection, and it can be, but once you get it working this means you just change TARGET at the top and build a whole new library.
Makefile:22
    Finally, to make the library you run ranlib on the TARGET and it's built.
Makefile:24-24
    This just makes the build/ or bin/ directories if they don't exist. This is then referenced from line 19 when it gives the build target to make sure the build/ directory is made.

You now have all the stuff you need to build the software, so we'll create a way to build and run unit tests to do automated testing.

# The Unit Tests

C is different from other languages because it's easier to create one tiny little program for each thing you're testing. Some testing frameworks try to emulate the module concept other languages have and do dynamic loading, but this doesn't work well in C. It's also unnecessary because you can just make a single program that's run for each test instead.

I'll cover this part of the Makefile, and then later you'll see the contents of the tests/ directory that make it actually work.

Makefile:29
    If you have a target that's not "real", but there is a directory or file with that name, then you need to tag the target with .PHONY: so make will ignore the file and always run.
Makefile:30
    I use the same trick for modifying the CFLAGS variable to add the TARGET to the build so that each of the test programs will be linked with the TARGET library. In this case it will add build/libYOUR_LIBRARY.a to the linking.
Makefile:31
    Then I have the actual tests: target which depends on all the programs listed in the TESTS variable we created in the header. This one line actually says, "Make, use what you know about building programs and the current CFLAGS settings to build each program in TESTS."
Makefile:32
    Finally, when all of the TESTS are built, there's a simple shell script I'll create later that knows how to run them all and report their output. This line actually runs it so you can see the test results.
Makefile:34-35
    In order to be able to dynamically rerun the tests with Valgrind there's a valgrind: target that sets the right variable and runs itself again. This puts the valgrind logs into /tmp/valgrind-*.log so you can go look and see what might be going on. The tests/runtests.sh then knows to run the test programs under Valgrind when it sees this VALGRIND variable.

For the unit testing to work you'll need to create a little shell script that knows how to run the programs. Go ahead and create this tests/runtests.sh script:
    
    echo "Running unit tests:"
    
    for i in tests/*_tests
    do
        if test -f $i
        then
            if $VALGRIND ./$i 2>> tests/tests.log
            then
                echo $i PASS
            else
                echo "ERROR in test $i: here's tests/tests.log"
                echo "------"
                tail tests/tests.log
                exit 1
            fi
        fi
    done
    
    echo ""
    

I'll be using this later when I cover how unit tests work.

# The Cleaner

I now have fully working unit tests, so next up is making things clean when I need to reset everything.

Makefile:38
    The clean: target starts things off whenever we need to clean up the project.
Makefile:39-42
    This cleans out most of the junk that various compilers and tools leave behind. It also gets rid of the build/ directory and uses a trick at the end to cleanly erase the weird *.dSYM directories Apple's XCode leaves behind for debugging purposes.

If you run into junk that you need to clean out, simply augment the list of things being deleted in this target.

# The Install

After that I'll need a way to install the project, and for a Makefile that's building a library I just need to put something in the common PREFIX directory, which is usually /usr/local/lib.

Makefile:45
    This makes install: depend on the all: target so that when you run make install it will be sure to build everything.
Makefile:46
    I then use the program install to create the target lib directory if it doesn't exist. In this case I'm trying to make the install as flexible as possible by using two variables that are conventions for installers. DESTDIR is handed to make by installers that do their builds in secure or odd locations to build packages. PREFIX is used when people want the project to be installed in someplace other than /usr/local.
Makefile:47
    After that I'm just using install to actually install the library where it needs to go.

The purpose of the install program is to make sure things have the right permissions set. When you run make install you usually have to do it as the root user, so the typical build process is make && sudo make install.

# The Checker

The very last part of this Makefile is a bonus that I include in my C projects to help me dig out any attempts to use the "bad" functions in C. Namely the string functions and other "unprotected buffer" functions.

Makefile:50
    Sets a variable which is a big regex looking for bad functions like strcpy.
Makefile:51
    The check: target so you can run a check whenever you need.
Makefile:52
    Just a way to print a message, but doing @echo tells make to not print the command, just its output.
Makefile:53
    Run the egrep command on the source files looking for any bad patterns. The || true at the end is a way to prevent make from thinking that egrep not finding things is a failure.

When you run this it will have the odd effect that you'll get an error when there is nothing bad going on.

# What You Should See

I have two more exercises to go before I'm done building the project skeleton directory, but here's me testing out the features of the Makefile.
    
    $ make clean
    rm -rf build  
    rm -f tests/tests.log 
    find . -name "*.gc*" -exec rm {} \;
    rm -rf `find . -name "*.dSYM" -print`
    $ make check
    Files with potentially dangerous functions.
    ^Cmake: *** [check] Interrupt: 2
    
    $ make
    ar rcs build/libYOUR_LIBRARY.a 
    ar: no archive members specified
    usage:  ar -d [-TLsv] archive file ...
          ar -m [-TLsv] archive file ...
          ar -m [-abiTLsv] position archive file ...
          ar -p [-TLsv] archive [file ...]
          ar -q [-cTLsv] archive file ...
          ar -r [-cuTLsv] archive file ...
          ar -r [-abciuTLsv] position archive file ...
          ar -t [-TLsv] archive [file ...]
          ar -x [-ouTLsv] archive [file ...]
    make: *** [build/libYOUR_LIBRARY.a] Error 1
    $ make valgrind
    VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" make
    ar rcs build/libYOUR_LIBRARY.a 
    ar: no archive members specified
    usage:  ar -d [-TLsv] archive file ...
          ar -m [-TLsv] archive file ...
          ar -m [-abiTLsv] position archive file ...
          ar -p [-TLsv] archive [file ...]
          ar -q [-cTLsv] archive file ...
          ar -r [-cuTLsv] archive file ...
          ar -r [-abciuTLsv] position archive file ...
          ar -t [-TLsv] archive [file ...]
          ar -x [-ouTLsv] archive [file ...]
    make[1]: *** [build/libYOUR_LIBRARY.a] Error 1
    make: *** [valgrind] Error 2
    $
    

When I run the clean: target that works, but because I don't have any source files in the src/ directory none of the other commands really work. I'll finish that up in the next exercises.

# Extra Credit

   Try to get the Makefile to actually work by putting a source and header file in src/ and making the library. You shouldn't need a main function in the source file.
   Research what functions the check: target is looking for in the BADFUNCS regular expression it's using.
   If you don't do automated unit testing, then go read about it so you're prepared later.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















