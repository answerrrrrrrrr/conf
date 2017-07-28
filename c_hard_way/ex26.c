Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 26: Write A First Real Program

You are at the half-way mark in the book, so you need to take a mid-term. In this mid-term you're going to recreate a piece of software I wrote specifically for this book called devpkg. You'll then extend it in a few key ways and improve the code, most importantly by writing some unit tests for it.

Note

I wrote this exercise before writing some of the exercises you might need to complete this. If you are attempting this one now, please keep in mind that the software may have bugs, that you might have problems because of my mistakes, and that you might not know everything you need to finish it. If so, tell me at help@learncodethehardway.org and then wait until I finish the other exercises.

# What Is devpkg?

Devpkg is a simple C program that installs other software. I made it specifically for this book as a way to teach you how a real software project is structured, and also how to reuse other people's libraries. It uses a portability library called The Apache Portable Runtime (APR) that has many handy C functions which work on tons of platforms, including Windows. Other than that, it just grabs code from the internet (or local files) and does the usual ./configure ; make ; make install every programmer does.

Your goal in this exercise is to build devpkg from source, finish each Challenge I give, and use the source to understand what devpkg does and why.

## What We Want To Make

We want a tool that has three commands:

devpkg -S
    Sets up a new install on a computer.
devpkg -I
    Installs a piece of software from a URL.
devpkg -L
    Lists all the software that's been installed.
devpkg -F
    Fetches some source code for manual building.
devpkg -B
    Builds fetches source code and installs it, even if already installed.

We want devpkg to be able to take almost any URL, figure out what kind of project it is, download it, install it, and register that it downloaded that software. We'd also like it to process a simple dependency list so it can install all the software that a project might need as well.

## The Design

To accomplish this goal devpkg will have a very simple design:

Use external commands
    You'll do most of the work through external commands like curl, git, and tar. This reduces the amount of code devpkg needs to get things done.
Simple File Database
    You could easily make it more complex, but to start you'll just make a single simple file database at /usr/local/.devpkg/db to keep track of what's installed.
/usr/local Always
    Again you could make this more advanced, but for starters just assume it's always /usr/local which is a standard install path for most software on Unix.
configure, make, make install
    It's assumed that most software can install with just a configure; make; make install and maybe configure is optional. If the software can't at a minimum do that, then there's some options to modify the commands, but otherwise devpkg won't bother.
The User Can Be root
    We'll assume the user can become root using sudo, but that they don't want to become root until the end.

This will keep our program small at first and work well enough to get it going, at which point you'll be able to modify it further for this exercise.

## The Apache Portable Runtime

One more thing you'll do is leverage the The Apache Portable Runtime (APR) libraries to get a good set of portable routines for doing this kind of work. The APR isn't necessary, and you could probably write this program without them, but it'd take more code than necessary. I'm also forcing you to use APR now so you get used to linking and using other libraries. Finally, the APR also works on Windows so your skills with it are transferable to many other platforms.

You should go get both the apr-1.4.5 and the apr-util-1.3 libraries, as well as browse through the documentation available at the main APR site at apr.apache.org

Here's a shell script that will install all the stuff you need. You should write this into a file by hand, and then run it until it can install APR without any errors.
    
    set -e
    
    # go somewhere safe
    cd /tmp
    
    # get the source to base APR 1.4.6
    curl -L -O http://archive.apache.org/dist/apr/apr-1.4.6.tar.gz
    
    # extract it and go into the source
    tar -xzvf apr-1.4.6.tar.gz
    cd apr-1.4.6
    
    # configure, make, make install
    ./configure
    make
    sudo make install
    
    # reset and cleanup
    cd /tmp
    rm -rf apr-1.4.6 apr-1.4.6.tar.gz
    
    # do the same with apr-util
    curl -L -O http://archive.apache.org/dist/apr/apr-util-1.4.1.tar.gz
    
    # extract
    tar -xzvf apr-util-1.4.1.tar.gz
    cd apr-util-1.4.1
    
    # configure, make, make install
    ./configure --with-apr=/usr/local/apr
    # you need that extra parameter to configure because
    # apr-util can't really find it because...who knows.
    
    make
    sudo make install
    
    #cleanup
    cd /tmp
    rm -rf apr-util-1.4.1* apr-1.4.6*
    

I'm having you write this script out because this is basically what we want devpkg to do, but with extra options and checks. In fact, you could just do it all in shell with less code, but then that wouldn't be a very good program for a C book would it?

Simply run this script and fix it until it works, then you'll have the libraries you need to complete the rest of this project.

# Project Layout

You need to setup some simple project files to get started. Here's how I usually craft a new project:
    
    mkdir devpkg
    cd devpkg
    touch README Makefile
    

## Other Dependencies

You should have already installed APR and APR-util, so now you need a few more files as basic dependencies:

   dbg.h from Exercise 20.
   bstrlib.h and bstrlib.c from http://bstring.sourceforge.net/. Download the .zip file, extract it, and copy just those two files out.
   Type make bstrlib.o and if it doesn't work, read the "Fixing bstring" instructions below.

Note

In some platforms the bstring.c file will have an error like:
    
    
    bstrlib.c:2762: error: expected declaration specifiers or '...' before numeric constant
    

This is from a bad define the authors added which doesn't work always. You just need to change the line 2759 that reads #ifdef __GNUC__ and make it:

#if defined(__GNUC__) && !defined(__APPLE__)

Then it should work on Apple Mac OSX.

When that's all done, you should have a Makefile, README, dbg.h, bstrlib.h, and bstrlib.c ready to go.

# The Makefile

A good place to start is the Makefile since this lays out how things are built and what source files you'll be creating.
    
    PREFIX?=/usr/local
    CFLAGS=-g -Wall -I${PREFIX}/apr/include/apr-1  -I${PREFIX}/apr/include/apr-util-1
    LDFLAGS=-L${PREFIX}/apr/lib -lapr-1 -pthread -laprutil-1
    
    all: devpkg
    
    devpkg: bstrlib.o db.o shell.o commands.o
    
    install: all
         install -d $(DESTDIR)/$(PREFIX)/bin/
         install devpkg $(DESTDIR)/$(PREFIX)/bin/
    
    clean:
         rm -f *.o
         rm -f devpkg
         rm -rf *.dSYM
    

There's nothing in this that you haven't seen before, except maybe the strange ?= syntax, which says "set PREFIX equal to this unless PREFIX is already set".

Note

If you are on more recent versions of Ubuntu and you get errors about apr_off_t or off64_t then add -D_LARGEFILE64_SOURCE=1 to CFLAGS.

Another thing is you need to add /usr/local/apr/lib to a file in /etc/ld.conf.so.d/ then run ldconfig so that it picks up the libraries correctly.

# The Source Files

From the make file, we see that there's four dependencies for devpkg which are:

bstrlib.o
    Comes from bstrlib.c and header file bstlib.h which you already have.
db.o
    From db.c and header file db.h, and it will contain code for our little "database" routines.
shell.o
    From shell.c and header shell.h, with a couple functions that make running other commands like curl easier.
commands.o
    From command.c and header command.h, and contains all the commands that devpkg needs to be useful.
devpkg
    It's not explicitly mentioned, but instead is the target (on the left) in this part of the Makefile. It comes from devpkg.c which contains the main function for the whole program.

Your job is to now create each of these files and type in their code and get them correct.

Note

You may read this description and think, "Man! How is it that Zed is so smart he just sat down and typed these files out like this!? I could never do that." I didn't magically craft devpkg in this form with my awesome code powers. Instead, what I did is this:

   I wrote a quick little README to get an idea of how I wanted it to work.
   I created a simple bash script (like the one you did) to figure out all the pieces that you need.
   I made one .c file and hacked on it for a few days working through the idea and figuring it out.
   I got it mostly working and debugged, then I started breaking up the one big file into these four files.
   After getting these files laid down, I renamed and refined the functions and data structures so they'd be more logical and "pretty".
   Finally, after I had it working the exact same but with the new structure, I added a few features like the -F and -B options.

You're reading this in the order I want to teach it to you, but don't think this is how I always build software. Sometimes I already know the subject and I use more planning. Sometimes I just hack up an idea and see how well it'd work. Sometimes I write one, then throw it away and plan out a better one. It all depends on what my experience tells me is best, or where my inspiration takes me.

If you run into an "expert" who tries to tell you there's only one way to solve a programming problem, then they're lying to you. Either they actually use multiple tactics, or they're not very good.

## The DB Functions

There must be a way to record URLs that have been installed, list these URLs, and check if something has already been installed so we can skip it. I'll use a simple flat file database and the bstrlib.h library to do it.

First, create the db.h header file so you know what you'll be implementing.
    
    #ifndef _db_h
    #define _db_h
    
    #define DB_FILE "/usr/local/.devpkg/db"
    #define DB_DIR "/usr/local/.devpkg"
    
    
    int DB_init();
    int DB_list();
    int DB_update(const char *url);
    int DB_find(const char *url);
    
    #endif
    

Then implement those functions in db.c, as you build this, use make like you've been to get it to compile cleanly.
    
    #include <unistd.h>
    #include <apr_errno.h>
    #include <apr_file_io.h>
    
    #include "db.h"
    #include "bstrlib.h"
    #include "dbg.h"
    
    static FILE *DB_open(const char *path, const char *mode)
    {
        return fopen(path, mode);
    }
    
    
    static void DB_close(FILE *db)
    {
        fclose(db);
    }
    
    
    static bstring DB_load()
    {
        FILE *db = NULL;
        bstring data = NULL;
    
        db = DB_open(DB_FILE, "r");
        check(db, "Failed to open database: %s", DB_FILE);
    
        data = bread((bNread)fread, db);
        check(data, "Failed to read from db file: %s", DB_FILE);
    
        DB_close(db);
        return data;
    
    error:
        if(db) DB_close(db);
        if(data) bdestroy(data);
        return NULL;
    }
    
    
    int DB_update(const char *url)
    {
        if(DB_find(url)) {
            log_info("Already recorded as installed: %s", url);
        }
    
        FILE *db = DB_open(DB_FILE, "a+");
        check(db, "Failed to open DB file: %s", DB_FILE);
    
        bstring line = bfromcstr(url);
        bconchar(line, '\n');
        int rc = fwrite(line->data, blength(line), 1, db);
        check(rc == 1, "Failed to append to the db.");
    
        return 0;
    error:
        if(db) DB_close(db);
        return -1;
    }
    
    
    int DB_find(const char *url)
    {
        bstring data = NULL;
        bstring line = bfromcstr(url);
        int res = -1;
    
        data = DB_load(DB_FILE);
        check(data, "Failed to load: %s", DB_FILE);
    
        if(binstr(data, 0, line) == BSTR_ERR) {
            res = 0;
        } else {
            res = 1;
        }
    
    error: // fallthrough
        if(data) bdestroy(data);
        if(line) bdestroy(line);
    
        return res;
    }
    
    
    int DB_init()
    {
        apr_pool_t *p = NULL;
        apr_pool_initialize();
        apr_pool_create(&p, NULL);
    
        if(access(DB_DIR, W_OK | X_OK) == -1) {
            apr_status_t rc = apr_dir_make_recursive(DB_DIR,
                    APR_UREAD | APR_UWRITE | APR_UEXECUTE |
                    APR_GREAD | APR_GWRITE | APR_GEXECUTE, p);
            check(rc == APR_SUCCESS, "Failed to make database dir: %s", DB_DIR);
        }
    
        if(access(DB_FILE, W_OK) == -1) {
            FILE *db = DB_open(DB_FILE, "w");
            check(db, "Cannot open database: %s", DB_FILE);
            DB_close(db);
        }
    
        apr_pool_destroy(p);
        return 0;
    
    error:
        apr_pool_destroy(p);
        return -1;
    }
    
    
    int DB_list()
    {
        bstring data = DB_load();
        check(data, "Failed to read load: %s", DB_FILE);
    
        printf("%s", bdata(data));
        bdestroy(data);
        return 0;
    
    error:
        return -1;
    }
    

### Challenge 1: Code Review

Before continuing, read every line of these files carefully and confirm that you have them entered in exactly. Read them line-by-line backwards to practice that. Also trace each function call and make sure you are using check to validate the return codes. Finally, look up every function that you don't recognize either on the APR web site documentation, or in the bstrlib.h and bstrlib.c source.

## The Shell Functions

A key design decision for devpkg is to do most of the work using external tools like curl, tar, and git. We could find libraries to do all of this internally, but it's pointless if we just need the base features of these programs. There is no shame in running another command in Unix.

To do this I'm going to use the apr_thread_proc.h functions to run programs, but I also want to make a simple kind of "template" system. I'll use a struct Shell that holds all the information needed to run a program, but has "holes" in the arguments list where I can replace them with values.

Look at the shell.h file to see the structure and the commands I'll use. You can see I'm using extern to indicate that other .c files can access variables I'm defining in shell.c.
    
    #ifndef _shell_h
    #define _shell_h
    
    #define MAX_COMMAND_ARGS 100
    
    #include <apr_thread_proc.h>
    
    typedef struct Shell {
        const char *dir;
        const char *exe;
    
        apr_procattr_t *attr;
        apr_proc_t proc;
        apr_exit_why_e exit_why;
        int exit_code;
    
        const char *args[MAX_COMMAND_ARGS];
    } Shell;
    
    int Shell_run(apr_pool_t *p, Shell *cmd);
    int Shell_exec(Shell cmd, ...);
    
    extern Shell CLEANUP_SH;
    extern Shell GIT_SH;
    extern Shell TAR_SH;
    extern Shell CURL_SH;
    extern Shell CONFIGURE_SH;
    extern Shell MAKE_SH;
    extern Shell INSTALL_SH;
    
    #endif
    

Make sure you've created shell.h exactly, and that you've got the same names and number of extern Shell variables. Those are used by the Shell_run and Shell_exec functions to run commands. I define these two functions, and create the real variables in shell.c.
    
    #include "shell.h"
    #include "dbg.h"
    #include <stdarg.h>
    
    int Shell_exec(Shell template, ...)
    {
        apr_pool_t *p = NULL;
        int rc = -1;
        apr_status_t rv = APR_SUCCESS;
        va_list argp;
        const char *key = NULL;
        const char *arg = NULL;
        int i = 0;
    
        rv = apr_pool_create(&p, NULL);
        check(rv == APR_SUCCESS, "Failed to create pool.");
    
        va_start(argp, template);
    
        for(key = va_arg(argp, const char *);
            key != NULL;
            key = va_arg(argp, const char *))
        {
            arg = va_arg(argp, const char *);
    
            for(i = 0; template.args[i] != NULL; i++) {
                if(strcmp(template.args[i], key) == 0) {
                    template.args[i] = arg;
                    break; // found it
                }
            }
        }
    
        rc = Shell_run(p, &template);
        apr_pool_destroy(p);
        va_end(argp);
        return rc;
    error:
        if(p) {
            apr_pool_destroy(p);
        }
        return rc;
    }
    
    int Shell_run(apr_pool_t *p, Shell *cmd)
    {
        apr_procattr_t *attr;
        apr_status_t rv;
        apr_proc_t newproc;
    
        rv = apr_procattr_create(&attr, p);
        check(rv == APR_SUCCESS, "Failed to create proc attr.");
    
        rv = apr_procattr_io_set(attr, APR_NO_PIPE, APR_NO_PIPE,
                APR_NO_PIPE);
        check(rv == APR_SUCCESS, "Failed to set IO of command.");
    
        rv = apr_procattr_dir_set(attr, cmd->dir);
        check(rv == APR_SUCCESS, "Failed to set root to %s", cmd->dir);
    
        rv = apr_procattr_cmdtype_set(attr, APR_PROGRAM_PATH);
        check(rv == APR_SUCCESS, "Failed to set cmd type.");
    
        rv = apr_proc_create(&newproc, cmd->exe, cmd->args, NULL, attr, p);
        check(rv == APR_SUCCESS, "Failed to run command.");
    
        rv = apr_proc_wait(&newproc, &cmd->exit_code, &cmd->exit_why, APR_WAIT);
        check(rv == APR_CHILD_DONE, "Failed to wait.");
    
        check(cmd->exit_code == 0, "%s exited badly.", cmd->exe);
        check(cmd->exit_why == APR_PROC_EXIT, "%s was killed or crashed", cmd->exe);
    
        return 0;
    
    error:
        return -1;
    }
    
    Shell CLEANUP_SH = {
        .exe = "rm",
        .dir = "/tmp",
        .args = {"rm", "-rf", "/tmp/pkg-build", "/tmp/pkg-src.tar.gz",
            "/tmp/pkg-src.tar.bz2", "/tmp/DEPENDS", NULL}
    };
    
    Shell GIT_SH = {
        .dir = "/tmp",
        .exe = "git",
        .args = {"git", "clone", "URL", "pkg-build", NULL}
    };
    
    Shell TAR_SH = {
        .dir = "/tmp/pkg-build",
        .exe = "tar",
        .args = {"tar", "-xzf", "FILE", "--strip-components", "1", NULL}
    };
    
    Shell CURL_SH = {
        .dir = "/tmp",
        .exe = "curl",
        .args = {"curl", "-L", "-o", "TARGET", "URL", NULL}
    };
    
    Shell CONFIGURE_SH = {
        .exe = "./configure",
        .dir = "/tmp/pkg-build",
        .args = {"configure", "OPTS", NULL},
    };
    
    Shell MAKE_SH = {
        .exe = "make",
        .dir = "/tmp/pkg-build",
        .args = {"make", "OPTS", NULL}
    };
    
    Shell INSTALL_SH = {
        .exe = "sudo",
        .dir = "/tmp/pkg-build",
        .args = {"sudo", "make", "TARGET", NULL}
    };
    

Read the shell.c from the bottom to the top (which is a common C source layout) and you see I've created the actual Shell variables that you indicated were extern in shell.h. They live here, but are available to the rest of the program. This is how you make global variables that live in one .o file but are used everywhere. You shouldn't make many of these, but they are handy for things like this.

Continuing up the file we get to the Shell_run function, which is a "base" function that just runs a command based on what's in a Shell struct. It uses many of the functions defined in apr_thread_proc.h so go look up each one to see how it works. This seems like a lot of work compared to just using the system function call, but this also gives you more control over the other program's execution. For example, in our Shell struct we have a .dir attribute which forces the program to be in a specific directory before running.

Finally, I have the Shell_exec function, which is a "variable arguments" function. You've seen this before, but make sure you grasp the stdarg.h functions and how to write one of these. In the challenge for this section you are going to analyze this function.

### Challenge 2: Analyze Shell_exec

Challenge for these files (in addition to a full code review just like you did in Challenge 1) is to fully analyze Shell_exec and break down exactly how it works. You should be able to understand each line, how the two for-loops work, and how arguments are being replaced.

Once you have it analyzed, add a field to struct Shell that gives the number of variable args that must be replaced. Update all the commands to have the right count of args, and then have an error check that confirms these args have been replaced and error exit.

## The Command Functions

Now you get to make the actual commands that do the work. These commands will use functions from APR, db.h and shell.h to do the real work of downloading and building software you want it to build. This is the most complex set of files, so do them carefully. As before, you start by making the commands.h file, then implementing its functions in the commands.c file.
    
    #ifndef _commands_h
    #define _commands_h
    
    #include <apr_pools.h>
    
    #define DEPENDS_PATH "/tmp/DEPENDS"
    #define TAR_GZ_SRC "/tmp/pkg-src.tar.gz"
    #define TAR_BZ2_SRC "/tmp/pkg-src.tar.bz2"
    #define BUILD_DIR "/tmp/pkg-build"
    #define GIT_PAT "*.git"
    #define DEPEND_PAT "*DEPENDS"
    #define TAR_GZ_PAT "*.tar.gz"
    #define TAR_BZ2_PAT "*.tar.bz2"
    #define CONFIG_SCRIPT "/tmp/pkg-build/configure"
    
    enum CommandType {
        COMMAND_NONE, COMMAND_INSTALL, COMMAND_LIST, COMMAND_FETCH,
        COMMAND_INIT, COMMAND_BUILD
    };
    
    
    int Command_fetch(apr_pool_t *p, const char *url, int fetch_only);
    
    int Command_install(apr_pool_t *p, const char *url, const char *configure_opts,
            const char *make_opts, const char *install_opts);
    
    int Command_depends(apr_pool_t *p, const char *path);
    
    int Command_build(apr_pool_t *p, const char *url, const char *configure_opts,
            const char *make_opts, const char *install_opts);
    
    #endif
    

There's not much in commands.h that you haven't seen already. You should see that there's some defines for strings that are used everywhere. The real interesting code is in commands.c.
    
    #include <apr_uri.h>
    #include <apr_fnmatch.h>
    #include <unistd.h>
    
    #include "commands.h"
    #include "dbg.h"
    #include "bstrlib.h"
    #include "db.h"
    #include "shell.h"
    
    
    int Command_depends(apr_pool_t *p, const char *path)
    {
        FILE *in = NULL;
        bstring line = NULL;
    
        in = fopen(path, "r");
        check(in != NULL, "Failed to open downloaded depends: %s", path);
    
        for(line = bgets((bNgetc)fgetc, in, '\n'); line != NULL;
                line = bgets((bNgetc)fgetc, in, '\n'))
        {
            btrimws(line);
            log_info("Processing depends: %s", bdata(line));
            int rc = Command_install(p, bdata(line), NULL, NULL, NULL);
            check(rc == 0, "Failed to install: %s", bdata(line));
            bdestroy(line);
        }
    
        fclose(in);
        return 0;
    
    error:
        if(line) bdestroy(line);
        if(in) fclose(in);
        return -1;
    }
    
    int Command_fetch(apr_pool_t *p, const char *url, int fetch_only)
    {
        apr_uri_t info = {.port = 0};
        int rc = 0;
        const char *depends_file = NULL;
        apr_status_t rv = apr_uri_parse(p, url, &info);
    
        check(rv == APR_SUCCESS, "Failed to parse URL: %s", url);
    
        if(apr_fnmatch(GIT_PAT, info.path, 0) == APR_SUCCESS) {
            rc = Shell_exec(GIT_SH, "URL", url, NULL);
            check(rc == 0, "git failed.");
        } else if(apr_fnmatch(DEPEND_PAT, info.path, 0) == APR_SUCCESS) {
            check(!fetch_only, "No point in fetching a DEPENDS file.");
    
            if(info.scheme) {
                depends_file = DEPENDS_PATH;
                rc = Shell_exec(CURL_SH, "URL", url, "TARGET", depends_file, NULL);
                check(rc == 0, "Curl failed.");
            } else {
                depends_file = info.path;
            }
    
            // recursively process the devpkg list
            log_info("Building according to DEPENDS: %s", url);
            rv = Command_depends(p, depends_file);
            check(rv == 0, "Failed to process the DEPENDS: %s", url);
    
            // this indicates that nothing needs to be done
            return 0;
    
        } else if(apr_fnmatch(TAR_GZ_PAT, info.path, 0) == APR_SUCCESS) {
            if(info.scheme) {
                rc = Shell_exec(CURL_SH,
                        "URL", url,
                        "TARGET", TAR_GZ_SRC, NULL);
                check(rc == 0, "Failed to curl source: %s", url);
            }
    
            rv = apr_dir_make_recursive(BUILD_DIR,
                    APR_UREAD | APR_UWRITE | APR_UEXECUTE, p);
            check(rv == APR_SUCCESS, "Failed to make directory %s", BUILD_DIR);
    
            rc = Shell_exec(TAR_SH, "FILE", TAR_GZ_SRC, NULL);
            check(rc == 0, "Failed to untar %s", TAR_GZ_SRC);
        } else if(apr_fnmatch(TAR_BZ2_PAT, info.path, 0) == APR_SUCCESS) {
            if(info.scheme) {
                rc = Shell_exec(CURL_SH, "URL", url, "TARGET", TAR_BZ2_SRC, NULL);
                check(rc == 0, "Curl failed.");
            }
    
            apr_status_t rc = apr_dir_make_recursive(BUILD_DIR,
                    APR_UREAD | APR_UWRITE | APR_UEXECUTE, p);
    
            check(rc == 0, "Failed to make directory %s", BUILD_DIR);
            rc = Shell_exec(TAR_SH, "FILE", TAR_BZ2_SRC, NULL);
            check(rc == 0, "Failed to untar %s", TAR_BZ2_SRC);
        } else {
            sentinel("Don't now how to handle %s", url);
        }
    
        // indicates that an install needs to actually run
        return 1;
    error:
        return -1;
    }
    
    int Command_build(apr_pool_t *p, const char *url, const char *configure_opts,
            const char *make_opts, const char *install_opts)
    {
        int rc = 0;
    
        check(access(BUILD_DIR, X_OK | R_OK | W_OK) == 0,
                "Build directory doesn't exist: %s", BUILD_DIR);
    
        // actually do an install
        if(access(CONFIG_SCRIPT, X_OK) == 0) {
            log_info("Has a configure script, running it.");
            rc = Shell_exec(CONFIGURE_SH, "OPTS", configure_opts, NULL);
            check(rc == 0, "Failed to configure.");
        }
    
        rc = Shell_exec(MAKE_SH, "OPTS", make_opts, NULL);
        check(rc == 0, "Failed to build.");
    
        rc = Shell_exec(INSTALL_SH,
                "TARGET", install_opts ? install_opts : "install",
                NULL);
        check(rc == 0, "Failed to install.");
    
        rc = Shell_exec(CLEANUP_SH, NULL);
        check(rc == 0, "Failed to cleanup after build.");
    
        rc = DB_update(url);
        check(rc == 0, "Failed to add this package to the database.");
    
        return 0;
    
    error:
        return -1;
    }
    
    int Command_install(apr_pool_t *p, const char *url, const char *configure_opts,
            const char *make_opts, const char *install_opts)
    {
        int rc = 0;
        check(Shell_exec(CLEANUP_SH, NULL) == 0, "Failed to cleanup before building.");
    
        rc = DB_find(url);
        check(rc != -1, "Error checking the install database.");
    
        if(rc == 1) {
            log_info("Package %s already installed.", url);
            return 0;
        }
    
        rc = Command_fetch(p, url, 0);
    
        if(rc == 1) {
            rc = Command_build(p, url, configure_opts, make_opts, install_opts);
            check(rc == 0, "Failed to build: %s", url);
        } else if(rc == 0) {
            // no install needed
            log_info("Depends successfully installed: %s", url);
        } else {
            // had an error
            sentinel("Install failed: %s", url);
        }
    
        Shell_exec(CLEANUP_SH, NULL);
        return 0;
    
    error:
        Shell_exec(CLEANUP_SH, NULL);
        return -1;
    }
    

After you have this entered in and compiling, you can analyze it. If you've don the challenges until now, you should see how the shell.c functions are being used to run shells and how the arguments are being replaced. If not then go back and make sure you truly understand how Shell_exec actually works.

### Challenge 3: Critique My Design

As before, do a complete review of this code and make sure it's exactly the same. Then go through each function and make sure you know how it works and what it's doing. You also should trace how each function calls the other functions you've written in this file and other files. Finally, confirm that you understand all the functions you're calling from APR here.

Once you have the file correct and analyzed, go back through and assume I'm an idiot. Then, criticize the design I have to see how you can improve it if you can. Don't actually change the code, just create a little notes.txt file and write down your thoughts and what you might change.

## The devpkg Main Function

The last and most important file, but probably the simplest, is devpkg.c where the main function lives. There's no .h file for this, since this one includes all the others. Instead this just creates the executable devpkg when combined with the other .o files from our Makefile. Enter in the code for this file, and make sure it's correct.
    
    #include <stdio.h>
    #include <apr_general.h>
    #include <apr_getopt.h>
    #include <apr_strings.h>
    #include <apr_lib.h>
    
    #include "dbg.h"
    #include "db.h"
    #include "commands.h"
    
    int main(int argc, const char const *argv[])
    {
        apr_pool_t *p = NULL;
        apr_pool_initialize();
        apr_pool_create(&p, NULL);
    
        apr_getopt_t *opt;
        apr_status_t rv;
    
        char ch = '\0';
        const char *optarg = NULL;
        const char *config_opts = NULL;
        const char *install_opts = NULL;
        const char *make_opts = NULL;
        const char *url = NULL;
        enum CommandType request = COMMAND_NONE;
    
    
        rv = apr_getopt_init(&opt, p, argc, argv);
    
        while(apr_getopt(opt, "I:Lc:m:i:d:SF:B:", &ch, &optarg) == APR_SUCCESS) {
            switch (ch) {
                case 'I':
                    request = COMMAND_INSTALL;
                    url = optarg;
                    break;
    
                case 'L':
                    request = COMMAND_LIST;
                    break;
    
                case 'c':
                    config_opts = optarg;
                    break;
    
                case 'm':
                    make_opts = optarg;
                    break;
    
                case 'i':
                    install_opts = optarg;
                    break;
    
                case 'S':
                    request = COMMAND_INIT;
                    break;
    
                case 'F':
                    request = COMMAND_FETCH;
                    url = optarg;
                    break;
    
                case 'B':
                    request = COMMAND_BUILD;
                    url = optarg;
                    break;
            }
        }
    
        switch(request) {
            case COMMAND_INSTALL:
                check(url, "You must at least give a URL.");
                Command_install(p, url, config_opts, make_opts, install_opts);
                break;
    
            case COMMAND_LIST:
                DB_list();
                break;
    
            case COMMAND_FETCH:
                check(url != NULL, "You must give a URL.");
                Command_fetch(p, url, 1);
                log_info("Downloaded to %s and in /tmp/", BUILD_DIR);
                break;
    
            case COMMAND_BUILD:
                check(url, "You must at least give a URL.");
                Command_build(p, url, config_opts, make_opts, install_opts);
                break;
    
            case COMMAND_INIT:
                rv = DB_init();
                check(rv == 0, "Failed to make the database.");
                break;
    
            default:
                sentinel("Invalid command given.");
        }
    
    
        return 0;
    
    error:
        return 1;
    }
    

### Challenge 4: The README And Test Files

The challenge for this file is to understand how the arguments are being processed, what the arguments are, and then create the README file with instructions on how to use it. As you write the README, also write a simple test.sh that runs ./devpkg to check that each command is actually working against real live code. Use the set -e at the top of your script so that it aborts on the first error.

Finally, run the program under valgrind and make sure it's all working before moving on to the mid-term exam.

# The Mid-Term Exam

Your final challenge is the mid-term exam and it involves three things:

   Compare your code to my code available online and starting with 100%, remove 1% for each line you got wrong.
   Take your notes.txt on how you would improve the code and functionality of devpkg and implement your improvements.
   Write an alternative version of devpkg using your other favorite language or the one you think can do this the best. Compare the two, then improve your C version of devpkg based on what you've learned.

To compare your code with mine, do the following:
    
    
    cd ..  # get one directory above your current one
    git clone git://gitorious.org/devpkg/devpkg.git devpkgzed
    diff -r devpkg devpkgzed
    

This will clone my version of devpkg into a directory devpkgzed and then use the tool diff to compare what you've done to what I did. The files you're working with in this book come directly from this project, so if you get different lines then that's an error.

Keep in mind that there's no real pass or fail on this exercise, just a way for you to challenge yourself to be as exact and meticulous as possible.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 



















