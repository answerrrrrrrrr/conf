Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 19: A Simple Object System

I learned C before I learned Object Oriented Programming, so it helped me to build an OOP system in C to understand the basics of what OOP meant. You are probably the kind of person who learned an OOP language before you learned C, so this kind of bridge might help you as well. In this exercise, you will build a simple object system, but also learn more about the C Pre-Processor or CPP.

This exercise will build a simple game where you kill a Minotaur in a small little castle. Nothing fancy, just four rooms and a bad guy. This project will also be a multi-file project, and look more like a real C software project than your previous ones. This is why I'm introducing the CPP here because you need it to start using multiple files in your own software.

# How The CPP Works

The C Pre-Processor is a template processing system. It's a highly targeted one that helps make C easier to work with, but it does this by having a syntax aware templating mechanism. Traditionally people just used the CPP to store constants and make "macros" to simplify repetitive coding. In modern C you'll actually use the CPP as a code generator to create templated pieces of code.

How the CPP works is you give it one file, usually a .c file, and it processes various bits of text starting with the # (octothorpe) character. When it encounters one of these it performs a specific replacement on the text of the input file. It's main advantage though is it can include other files, and then augment its list of macros based on that file's contents.

A quick way to see what the CPP does is take the last exercise and run this:
    
    
    cpp ex18.c | less
    

It will be a huge amount of output, but scroll through it and you'll see the contents of the other files you included with #include. Scroll down to the original code and you can see how the cpp is altering the source based on various #define macros in the header files.

The C compiler is so tightly integrated with cpp that it just runs this for you and understands how it works intimately. In modern C, the cpp system is so integral to C's function that you might as well just consider it to be part of the language.

In the remaining sections, we'll be using more of the CPP syntax and explaining it as we go.

# The Prototype Object System

The OOP system we'll create is a simple "prototype" style object system more like JavaScript. Instead of classes, you start with prototypes that have fields set, and then use those as the basis of creating other object instances. This "classless" design is much easier to implement and work with than a traditional class based one.

## The Object Header File

I want to put the data types and function declarations into a separate header file named object.h. This is standard C practice and it lets you ship binary libraries but still let the programmer compile against it. In this file I have several advanced CPP techniques I'm going to quickly describe and then have you see in action later:
    
    #ifndef _object_h
    #define _object_h
    
    typedef enum {
        NORTH, SOUTH, EAST, WEST
    } Direction;
    
    typedef struct {
        char *description;
        int (*init)(void *self);
        void (*describe)(void *self);
        void (*destroy)(void *self);
        void *(*move)(void *self, Direction direction);
        int (*attack)(void *self, int damage);
    } Object;
    
    int Object_init(void *self);
    void Object_destroy(void *self);
    void Object_describe(void *self);
    void *Object_move(void *self, Direction direction);
    int Object_attack(void *self, int damage);
    void *Object_new(size_t size, Object proto, char *description);
    
    #define NEW(T, N) Object_new(sizeof(T), T##Proto, N)
    #define _(N) proto.N
    
    #endif
    

Taking a look at this file, you can see we have a few new pieces of syntax you haven't encountered before:

#ifndef
    You've seen a #define for making simple constants, but the CPP can also do logic and remove sections of code. This #ifndef is "if not defined" and checks if there's already a #define _object_h and if there is it skips all of this code. I do this so that we can include this file any time we want and not worry about it defining things multiple times.
#define
    With the above #ifndef shielding this file from we then add the _object_h define so that any attempts to include it later cause the above to skip.
#define NEW(T,N)
    This makes a macro, and it works like a template function that spits out the code on the right, whenever you write use the macro on the left. This one is simply making a short version of the normal way we'll call Object_new and avoids potential errors with calling it wrong. The way the macro works is the T and N parameters to NEW are "injected" into the line of code on the right. The syntax T##Proto says to "concat Proto at the end of T", so if you had NEW(Room, "Hello.") then it'd make RoomProto there.
#define _(N)
    This macro is a bit of "syntactic sugar" for the object system and basically helps you write obj->proto.blah as simply obj->_(blah). It's not necessary, but it's a fun little trick that I'll use later.

## The Object Source File

The object.h file is declaring functions and data types that are defined (created) in the object.c, so that's next:
    
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include "object.h"
    #include <assert.h>
    
    void Object_destroy(void *self)
    {
        Object *obj = self;
    
        if(obj) {
            if(obj->description) free(obj->description);
            free(obj);
        }
    }
    
    void Object_describe(void *self)
    {
        Object *obj = self;
        printf("%s.\n", obj->description);
    }
    
    int Object_init(void *self)
    {
        // do nothing really
        return 1;
    }
    
    void *Object_move(void *self, Direction direction)
    {
        printf("You can't go that direction.\n");
        return NULL;
    }
    
    int Object_attack(void *self, int damage)
    {
        printf("You can't attack that.\n");
        return 0;
    }
    
    void *Object_new(size_t size, Object proto, char *description)
    {
        // setup the default functions in case they aren't set
        if(!proto.init) proto.init = Object_init;
        if(!proto.describe) proto.describe = Object_describe;
        if(!proto.destroy) proto.destroy = Object_destroy;
        if(!proto.attack) proto.attack = Object_attack;
        if(!proto.move) proto.move = Object_move;
    
        // this seems weird, but we can make a struct of one size,
        // then point a different pointer at it to "cast" it
        Object *el = calloc(1, size);
        *el = proto;
    
        // copy the description over
        el->description = strdup(description);
    
        // initialize it with whatever init we were given
        if(!el->init(el)) {
            // looks like it didn't initialize properly
            el->destroy(el);
            return NULL;
        } else {
            // all done, we made an object of any type
            return el;
        }
    }
    

There's really nothing new in this file, except one tiny little trick. The function Object_new uses an aspect of how structs work by putting the base prototype at the beginning of the struct. When you look at the ex19.h header later, you'll see how I make the first field in the struct an Object. Since C puts the fields in a struct in order, and since a pointer just points at a chunk of memory, I can "cast" a pointer to anything I want. In this case, even though I'm taking a potentially larger block of memory from calloc, I'm using a Object pointer to work with it.

I explain this a bit better when we write the ex19.h file since it's easier to understand when you see it being used.

That creates your base object system, but you'll need a way to compile it and link it into your ex19.c file to create a complete program. The object.c file on its own doesn't have a main so it isn't enough to make a full program. Here's a Makefile that will do this based on the one you've been using:
    
    CFLAGS=-Wall -g
    
    all: ex19
    
    ex19: object.o
    
    clean:
      rm -f ex19
    

This Makefile is doing nothing more than saying that ex19 depends on object.o. Remember how make knows how to build different kinds of files by their extensions? Doing this tells make the following:

   When I say run make the default all should just build ex19.
   When you build ex19, you need to also build object.o and include it in the build.
   make can't see anything in the file for object.o, but it does see an object.c file, and it knows how to turn a .c into a .o, so it does that.
   Once it has object.o built it then runs the correct compile command to build ex19 from ex19.c and object.o.

# The Game Implementation

Once you have those files you just need to implement the actual game using the object system, and first step is putting all the data types and function declarations in a ex19.h file:
    
    #ifndef _ex19_h
    #define _ex19_h
    
    #include "object.h"
    
    struct Monster {
        Object proto;
        int hit_points;
    };
    
    typedef struct Monster Monster;
    
    int Monster_attack(void *self, int damage);
    int Monster_init(void *self);
    
    struct Room {
        Object proto;
    
        Monster *bad_guy;
    
        struct Room *north;
        struct Room *south;
        struct Room *east;
        struct Room *west;
    };
    
    typedef struct Room Room;
    
    void *Room_move(void *self, Direction direction);
    int Room_attack(void *self, int damage);
    int Room_init(void *self);
    
    
    struct Map {
        Object proto;
        Room *start;
        Room *location;
    };
    
    typedef struct Map Map;
    
    void *Map_move(void *self, Direction direction);
    int Map_attack(void *self, int damage);
    int Map_init(void *self);
    
    #endif
    

That sets up three new Objects you'll be using: Monster, Room, and Map.

Taking a look at object.c:52 you can see where I use a pointer Object *el = calloc(1, size). Go back and look at the NEW macro in object.h and you can see that it is getting the sizeof another struct, say Room, and I allocate that much. However, because I've pointed a Object pointer at this block of memory, and because I put an Object proto field at the front of Room, I'm able to treat a Room like it's an Object.

The way to break this down is like so:

   I call NEW(Room, "Hello.") which the CPP expands as a macro into Object_new(sizeof(Room), RoomProto, "Hello.").
   This runs, and inside Object_new I allocate a piece of memory that's Room in size, but point a Object *el pointer at it.
   Since C puts the Room.proto field first, that means the el pointer is really only pointing at enough of the block of memory to see a full Object struct. It has no idea that it's even called proto.
   It then uses this Object *el pointer to set the contents of the piece of memory correctly with *el = proto;. Remember that you can copy structs, and that *el means "the value of whatever el points at", so this means "assign the proto struct to whatever el points at".
   Now that this mystery struct is filled in with the right data from proto, the function can then call init or destroy on the Object, but the cool part is whoever called this function can change these out for whatever ones they want.

And with that, we have a way to get this one function to construct new types, and give them new functions to change their behavior. This may seem like "hackery" but it's stock C and totally valid. In fact there's quite a few standard system functions that work this same way, and we'll be using some of them for converting addresses in network code.

With the function definitions and data structures written out I can now actually implement the game with four rooms and a minotaur to beat up:
    
    #include <stdio.h>
    #include <errno.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>
    #include "ex19.h"
    
    
    int Monster_attack(void *self, int damage)
    {
        Monster *monster = self;
    
        printf("You attack %s!\n", monster->_(description));
    
        monster->hit_points -= damage;
    
        if(monster->hit_points > 0) {
            printf("It is still alive.\n");
            return 0;
        } else {
            printf("It is dead!\n");
            return 1;
        }
    }
    
    int Monster_init(void *self)
    {
        Monster *monster = self;
        monster->hit_points = 10;
        return 1;
    }
    
    Object MonsterProto = {
        .init = Monster_init,
        .attack = Monster_attack
    };
    
    
    void *Room_move(void *self, Direction direction)
    {
        Room *room = self;
        Room *next = NULL;
    
        if(direction == NORTH && room->north) {
            printf("You go north, into:\n");
            next = room->north;
        } else if(direction == SOUTH && room->south) {
            printf("You go south, into:\n");
            next = room->south;
        } else if(direction == EAST && room->east) {
            printf("You go east, into:\n");
            next = room->east;
        } else if(direction == WEST && room->west) {
            printf("You go west, into:\n");
            next = room->west;
        } else {
            printf("You can't go that direction.");
            next = NULL;
        }
    
        if(next) {
            next->_(describe)(next);
        }
    
        return next;
    }
    
    
    int Room_attack(void *self, int damage)
    {
        Room *room = self;
        Monster *monster = room->bad_guy;
    
        if(monster) {
            monster->_(attack)(monster, damage);
            return 1;
        } else {
            printf("You flail in the air at nothing. Idiot.\n");
            return 0;
        }
    }
    
    
    Object RoomProto = {
        .move = Room_move,
        .attack = Room_attack
    };
    
    
    void *Map_move(void *self, Direction direction)
    {
        Map *map = self;
        Room *location = map->location;
        Room *next = NULL;
    
        next = location->_(move)(location, direction);
    
        if(next) {
            map->location = next;
        }
    
        return next;
    }
    
    int Map_attack(void *self, int damage)
    {
        Map* map = self;
        Room *location = map->location;
    
        return location->_(attack)(location, damage);
    }
    
    
    int Map_init(void *self)
    {
        Map *map = self;
    
        // make some rooms for a small map
        Room *hall = NEW(Room, "The great Hall");
        Room *throne = NEW(Room, "The throne room");
        Room *arena = NEW(Room, "The arena, with the minotaur");
        Room *kitchen = NEW(Room, "Kitchen, you have the knife now");
    
        // put the bad guy in the arena
        arena->bad_guy = NEW(Monster, "The evil minotaur");
    
        // setup the map rooms
        hall->north = throne;
    
        throne->west = arena;
        throne->east = kitchen;
        throne->south = hall;
    
        arena->east = throne;
        kitchen->west = throne;
    
        // start the map and the character off in the hall
        map->start = hall;
        map->location = hall;
    
        return 1;
    }
    
    Object MapProto = {
        .init = Map_init,
        .move = Map_move,
        .attack = Map_attack
    };
    
    int process_input(Map *game)
    {
        printf("\n> ");
    
        char ch = getchar();
        getchar(); // eat ENTER
    
        int damage = rand() % 4;
    
        switch(ch) {
            case -1:
                printf("Giving up? You suck.\n");
                return 0;
                break;
    
            case 'n':
                game->_(move)(game, NORTH);
                break;
    
            case 's':
                game->_(move)(game, SOUTH);
                break;
    
            case 'e':
                game->_(move)(game, EAST);
                break;
    
            case 'w':
                game->_(move)(game, WEST);
                break;
    
            case 'a':
    
                game->_(attack)(game, damage);
                break;
            case 'l':
                printf("You can go:\n");
                if(game->location->north) printf("NORTH\n");
                if(game->location->south) printf("SOUTH\n");
                if(game->location->east) printf("EAST\n");
                if(game->location->west) printf("WEST\n");
                break;
    
            default:
                printf("What?: %d\n", ch);
        }
    
        return 1;
    }
    
    int main(int argc, char *argv[])
    {
        // simple way to setup the randomness
        srand(time(NULL));
    
        // make our map to work with
        Map *game = NEW(Map, "The Hall of the Minotaur.");
    
        printf("You enter the ");
        game->location->_(describe)(game->location);
    
        while(process_input(game)) {
        }
    
        return 0;
    }
    

Honestly there isn't much in this that you haven't seen, and only you might need to understand how I'm using the macros I made from the headers files. Here's the important key things to study and understand:

   Implementing a prototype involves creating its version of the functions, and then creating a single struct ending in "Proto". Look at MonsterProto, RoomProto and MapProto.
   Because of how Object_new is implemented, if you don't set a function in your prototype, then it will get the default implementation created in object.c.
   In Map_init I create the little world, but more importantly I use the NEW macro from object.h to build all of the objects. To get this concept in your head, try replacing the NEW usage with direct Object_new calls to see how it's being translated.
   Working with these objects involves calling functions on them, and the _(N) macro does this for me. If you look at the code monster->_(attack)(monster, damage) you see that I'm using the macro, which gets replaced with monster->proto.attack(monster, damage). Study this transformation again by rewriting these calls back to their original. Also, if you get stuck then run cpp manually to see what it's going to do.
   I'm using two new functions srand and rand, which setup a simple random number generator good enough for the game. I also use time to initialize the random number generator. Research those.
   I use a new function getchar that gets a single character from the stdin. Research it.

# What You Should See

Here's me playing my own game:
    
    $ make ex19
    cc -Wall -g   -c -o object.o object.c
    cc -Wall -g    ex19.c object.o   -o ex19
    $ ./ex19
    You enter the The great Hall.
    
    > l
    You can go:
    NORTH
    
    > n
    You go north, into:
    The throne room.
    
    > l
    You can go:
    SOUTH
    EAST
    WEST
    
    > e
    You go east, into:
    Kitchen, you have the knife now.
    
    > w
    You go west, into:
    The throne room.
    
    > s
    You go south, into:
    The great Hall.
    
    > n
    You go north, into:
    The throne room.
    
    > w
    You go west, into:
    The arena, with the minotaur.
    
    > a
    You attack The evil minotaur!
    It is still alive.
    
    > a
    You attack The evil minotaur!
    It is dead!
    
    > ^D
    Giving up? You suck.
    $
    

# Auditing The Game

As an exercise for you I have left out all of the assert checks I normally put into a piece of software. You've seen me use assert to make sure a program is running correctly, but now I want you to go back and do the following:

   Look at each function you've defined, one file at a time.
   At the top of each function, add asserts that make sure the input parameters are correct. For example, in Object_new you want a assert(description != NULL).
   Go through each line of the function, and find any functions being called. Read the documentation (man page) for that function, and confirm what it returns for an error. Add another assert to check that the error didn't happen. For example, in Object_new you need one after the call to calloc that does assert(el != NULL).
   If a function is expected to return a value, either make sure it returns an error value (like NULL), or have an assert to make sure that the returned variable isn't invalid. For example, in Object_new, you need to have assert(el != NULL) again before the last return since that part can never be NULL.
   For every if-statement you write, make sure there's an else clause unless that if is an error check that causes an exit.
   For every switch-statement you write, make sure that there's a default case that handles anything you didn't anticipate.

Take your time going through every line of the function and find any errors you make. Remember that the point of this exercise is to stop being a "coder" and switch your brain into being a "hacker". Try to see how you could break it, then write code to prevent it or abort early if you can.

# Extra Credit

   Update the Makefile so that when you do make clean it will also remove the object.o file.
   Write a test script that works the game in different ways and augment the Makefile so you can run make test and it'll thrash the game with your script.
   Add more rooms and monsters to the game.
   Put the game mechanics into a third file, compile it to .o, and then use that to write another little game. If you're doing it right you should only have a new Map and a main function in the new game.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















