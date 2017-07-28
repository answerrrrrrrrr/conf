Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 45: A Simple TCP/IP Client

I'm going to use the RingBuffer to create a very simplistic little network testing tool called netclient. To do this I have to add some stuff to the Makefile to handle little programs in the bin/ directory.

# Augment The Makefile

First, add a variable for the programs just like the unit tests TESTS and TEST_SRC variables:
    
    
    PROGRAMS_SRC=$(wildcard bin/*.c)
    PROGRAMS=$(patsubst %.c,%,$(PROGRAMS_SRC))
    

Then you want to add the PROGRAMS to the all target:
    
    
    all: $(TARGET) $(SO_TARGET) tests $(PROGRAMS)
    

Then add PROGRAMS to the rm line in the clean target:
    
    
    rm -rf build $(OBJECTS) $(TESTS) $(PROGRAMS)
    

Finally you just need a target at the end to build them all:
    
    
    $(PROGRAMS): CFLAGS += $(TARGET)
    

With these changes you can drop simple .c files into bin and make will build them and link them to the library just like the tests are done.

# The netclient Code

The code for the little netclient looks like this:
    
    #undef NDEBUG
    #include <stdlib.h>
    #include <sys/select.h>
    #include <stdio.h>
    #include <lcthw/ringbuffer.h>
    #include <lcthw/dbg.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <sys/uio.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <fcntl.h>
    
    struct tagbstring NL = bsStatic("\n");
    struct tagbstring CRLF = bsStatic("\r\n");
    
    int nonblock(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        check(flags >= 0, "Invalid flags on nonblock.");
    
        int rc = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        check(rc == 0, "Can't set nonblocking.");
    
        return 0;
    error:
        return -1;
    }
    
    int client_connect(char *host, char *port)
    {
        int rc = 0;
        struct addrinfo *addr = NULL;
    
        rc = getaddrinfo(host, port, NULL, &addr);
        check(rc == 0, "Failed to lookup %s:%s", host, port);
    
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        check(sock >= 0, "Cannot create a socket.");
    
        rc = connect(sock, addr->ai_addr, addr->ai_addrlen);
        check(rc == 0, "Connect failed.");
    
        rc = nonblock(sock);
        check(rc == 0, "Can't set nonblocking.");
    
        freeaddrinfo(addr);
        return sock;
    
    error:
        freeaddrinfo(addr);
        return -1;
    }
    
    int read_some(RingBuffer *buffer, int fd, int is_socket)
    {
        int rc = 0;
    
        if(RingBuffer_available_data(buffer) == 0) {
            buffer->start = buffer->end = 0;
        }
    
        if(is_socket) {
            rc = recv(fd, RingBuffer_starts_at(buffer), RingBuffer_available_space(buffer), 0);
        } else {
            rc = read(fd, RingBuffer_starts_at(buffer), RingBuffer_available_space(buffer));
        }
    
        check(rc >= 0, "Failed to read from fd: %d", fd);
    
        RingBuffer_commit_write(buffer, rc);
    
        return rc;
    
    error:
        return -1;
    }
    
    
    int write_some(RingBuffer *buffer, int fd, int is_socket)
    {
        int rc = 0;
        bstring data = RingBuffer_get_all(buffer);
    
        check(data != NULL, "Failed to get from the buffer.");
        check(bfindreplace(data, &NL, &CRLF, 0) == BSTR_OK, "Failed to replace NL.");
    
        if(is_socket) {
            rc = send(fd, bdata(data), blength(data), 0);
        } else {
            rc = write(fd, bdata(data), blength(data));
        }
    
        check(rc == blength(data), "Failed to write everything to fd: %d.", fd);
        bdestroy(data);
    
        return rc;
    
    error:
        return -1;
    }
    
    
    int main(int argc, char *argv[])
    {
        fd_set allreads;
        fd_set readmask;
    
        int socket = 0;
        int rc = 0;
        RingBuffer *in_rb = RingBuffer_create(1024 * 10);
        RingBuffer *sock_rb = RingBuffer_create(1024 * 10);
    
        check(argc == 3, "USAGE: netclient host port");
    
        socket = client_connect(argv[1], argv[2]);
        check(socket >= 0, "connect to %s:%s failed.", argv[1], argv[2]);
    
        FD_ZERO(&allreads);
        FD_SET(socket, &allreads);
        FD_SET(0, &allreads);
    
        while(1) {
            readmask = allreads;
            rc = select(socket + 1, &readmask, NULL, NULL, NULL);
            check(rc >= 0, "select failed.");
    
            if(FD_ISSET(0, &readmask)) {
                rc = read_some(in_rb, 0, 0);
                check_debug(rc != -1, "Failed to read from stdin.");
            }
    
            if(FD_ISSET(socket, &readmask)) {
                rc = read_some(sock_rb, socket, 0);
                check_debug(rc != -1, "Failed to read from socket.");
            }
    
            while(!RingBuffer_empty(sock_rb)) {
                rc = write_some(sock_rb, 1, 0);
                check_debug(rc != -1, "Failed to write to stdout.");
            }
    
            while(!RingBuffer_empty(in_rb)) {
                rc = write_some(in_rb, socket, 1);
                check_debug(rc != -1, "Failed to write to socket.");
            }
        }
    
        return 0;
    
    error:
        return -1;
    }
    

This code uses select to handle events from both stdin (file descriptor 0) and from the socket it uses to talk to a server. It uses RingBuffers to store the data and copy it around, and you can consider the functions read_some and write_some early prototypes for similar functions in the RingBuffer library.

In this little bit of code are quite a few networking functions you may not know. As you hit a function you don't know, look it up in the man pages and make sure you understand it. This one little file could actually get you to research all the APIs required to write a little server in C.

# What You Should See

If you have everything building then the quickest way to test it is see if you can get a special file off learncodethehardway.org:
    
    $
    $ ./bin/netclient learncodethehardway.org 80
    GET /ex45.txt HTTP/1.1
    Host: learncodethehardway.org
    
    HTTP/1.1 200 OK
    Date: Fri, 27 Apr 2012 00:41:25 GMT
    Content-Type: text/plain
    Content-Length: 41
    Last-Modified: Fri, 27 Apr 2012 00:42:11 GMT
    ETag: 4f99eb63-29
    Server: Mongrel2/1.7.5
    
    Learn C The Hard Way, Exercise 45 works.
    ^C
    $
    

What I did there is I type in the syntax needed to make the HTTP request for the file /ex45.txt, then the Host: header line, then hit ENTER to get an empty line. I then get the response, with headers and the content. After that I just hit CTRL-c to exit.

# How To Break It

This code definitely could have bugs, and currently in the draft of the book I'm going to have to keep working on this. In the meantime, try analyzing the code I have here and thrashing it against other servers. There's a tool called netcat that is great for setting up these kinds of servers. Another thing to do is use a language like Python or Ruby to create a simple "junk server" that spews out junk and bad data, closes connections randomly, and other nasty things.

If you find bugs, report them in the comments and I'll fix them up.

# Extra Credit

   As I mentioned, there's quite a few functions you may not know, so look them up. In fact, look them all up even if you think you know them.
   Run this under valgrind and look for errors.
   Go back through and add various defensive programming checks to the functions to improve them.
   Use the getopt function to allow the user to give this the option to not translate \n to \r\n. This is only needed on protocols that require it for line endings, like HTTP. Sometimes you don't want the translation, so give the user an option.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















