Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 44: Ring Buffer

Ring buffers are incredibly useful when processing asynchronous IO. They allow one side to receive data in random intervals of random sizes, but feed cohesive chunks to another side in set sizes or intervals. They are a variant on the Queue data structure but it focuses on blocks of bytes instead of a list of pointers. In this exercise I'm going to show you the RingBuffer code, and then you have to make a full unit test for it.
    
    #ifndef _lcthw_RingBuffer_h
    #define _lcthw_RingBuffer_h
    
    #include <lcthw/bstrlib.h>
    
    typedef struct {
        char *buffer;
        int length;
        int start;
        int end;
    } RingBuffer;
    
    RingBuffer *RingBuffer_create(int length);
    
    void RingBuffer_destroy(RingBuffer *buffer);
    
    int RingBuffer_read(RingBuffer *buffer, char *target, int amount);
    
    int RingBuffer_write(RingBuffer *buffer, char *data, int length);
    
    int RingBuffer_empty(RingBuffer *buffer);
    
    int RingBuffer_full(RingBuffer *buffer);
    
    int RingBuffer_available_data(RingBuffer *buffer);
    
    int RingBuffer_available_space(RingBuffer *buffer);
    
    bstring RingBuffer_gets(RingBuffer *buffer, int amount);
    
    #define RingBuffer_available_data(B) (((B)->end + 1) % (B)->length - (B)->start - 1)
    
    #define RingBuffer_available_space(B) ((B)->length - (B)->end - 1)
    
    #define RingBuffer_full(B) (RingBuffer_available_data((B)) - (B)->length == 0)
    
    #define RingBuffer_empty(B) (RingBuffer_available_data((B)) == 0)
    
    #define RingBuffer_puts(B, D) RingBuffer_write((B), bdata((D)), blength((D)))
    
    #define RingBuffer_get_all(B) RingBuffer_gets((B), RingBuffer_available_data((B)))
    
    #define RingBuffer_starts_at(B) ((B)->buffer + (B)->start)
    
    #define RingBuffer_ends_at(B) ((B)->buffer + (B)->end)
    
    #define RingBuffer_commit_read(B, A) ((B)->start = ((B)->start + (A)) % (B)->length)
    
    #define RingBuffer_commit_write(B, A) ((B)->end = ((B)->end + (A)) % (B)->length)
    
    #endif
    

Looking at the data structure you see I have a buffer, start and end. A RingBuffer does nothing more than move the start and end around the buffer so that it "loops" whenever it reaches the buffer's end. Doing this gives the illusion of an infinite read device in a small space. I then have a bunch of macros that do various calculations based on this.

Here's the implementation which is a much better explanation of how this works:
    
    #undef NDEBUG
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <lcthw/dbg.h>
    #include <lcthw/ringbuffer.h>
    
    RingBuffer *RingBuffer_create(int length)
    {
        RingBuffer *buffer = calloc(1, sizeof(RingBuffer));
        buffer->length  = length + 1;
        buffer->start = 0;
        buffer->end = 0;
        buffer->buffer = calloc(buffer->length, 1);
    
        return buffer;
    }
    
    void RingBuffer_destroy(RingBuffer *buffer)
    {
        if(buffer) {
            free(buffer->buffer);
            free(buffer);
        }
    }
    
    int RingBuffer_write(RingBuffer *buffer, char *data, int length)
    {
        if(RingBuffer_available_data(buffer) == 0) {
            buffer->start = buffer->end = 0;
        }
    
        check(length <= RingBuffer_available_space(buffer),
                "Not enough space: %d request, %d available",
                RingBuffer_available_data(buffer), length);
    
        void *result = memcpy(RingBuffer_ends_at(buffer), data, length);
        check(result != NULL, "Failed to write data into buffer.");
    
        RingBuffer_commit_write(buffer, length);
    
        return length;
    error:
        return -1;
    }
    
    int RingBuffer_read(RingBuffer *buffer, char *target, int amount)
    {
        check_debug(amount <= RingBuffer_available_data(buffer),
                "Not enough in the buffer: has %d, needs %d",
                RingBuffer_available_data(buffer), amount);
    
        void *result = memcpy(target, RingBuffer_starts_at(buffer), amount);
        check(result != NULL, "Failed to write buffer into data.");
    
        RingBuffer_commit_read(buffer, amount);
    
        if(buffer->end == buffer->start) {
            buffer->start = buffer->end = 0;
        }
    
        return amount;
    error:
        return -1;
    }
    
    bstring RingBuffer_gets(RingBuffer *buffer, int amount)
    {
        check(amount > 0, "Need more than 0 for gets, you gave: %d ", amount);
        check_debug(amount <= RingBuffer_available_data(buffer),
                "Not enough in the buffer.");
    
        bstring result = blk2bstr(RingBuffer_starts_at(buffer), amount);
        check(result != NULL, "Failed to create gets result.");
        check(blength(result) == amount, "Wrong result length.");
    
        RingBuffer_commit_read(buffer, amount);
        assert(RingBuffer_available_data(buffer) >= 0 && "Error in read commit.");
    
        return result;
    error:
        return NULL;
    }
    

This is all there is to a basic RingBuffer implementation. You can read and write blocks of data to it. You can ask how much is in it and how much space it has. There are some fancier ring buffers that use tricks in the OS to create an imaginary infinite store, but those aren't portable.

Since my RingBuffer deals with reading and writing blocks of memory, I'm making sure that any time end == start then I reset them to 0 (zero) so that they go to the beginning of the buffer. In the Wikipedia version it wasn't writing blocks of data, so it only had to move end and start around in a circle. To better handle blocks you have to drop to the beginning of the internal buffer whenever the data is empty.

# The Unit Test

For your unit test, you'll want to test as many possible conditions as you can. Easiest way to do that is to preconstruct different RingBuffer structs and then manually check that the functions and math work right. For example, you could make one where end is right at the end of the buffer and start is right before it, then see how it fails.

# What You Should See

Here's my ringbuffer_tests run:
    
    $ ./tests/ringbuffer_tests
    DEBUG tests/ringbuffer_tests.c:60: ----- RUNNING: ./tests/ringbuffer_tests
    ----
    RUNNING: ./tests/ringbuffer_tests
    DEBUG tests/ringbuffer_tests.c:53: 
    ----- test_create
    DEBUG tests/ringbuffer_tests.c:54: 
    ----- test_read_write
    DEBUG tests/ringbuffer_tests.c:55: 
    ----- test_destroy
    ALL TESTS PASSED
    Tests run: 3
    $
    

You should have at least three tests that confirm all the basic operations, and then see how much more you can test beyond what I've done.

# How To Improve It

As usual you should go back and add the defensive programming checks to this exercise. Hopefully you've been doing this because the base code in most of liblcthw doesn't check for common defensive programming that I'm teaching you. I leave this to you so that you get used to improving code with these extra checks.

For example, in this ring buffer there's not a lot of checking that an access will actually be inside the buffer.

If you read the Ring Buffer Wikipedia page you'll see the "Optimized POSIX implementation" that uses POSIX specific calls to create an infinite space. Study that as I'll have you try it in the extra credit.

# Extra Credit

   Create an alternative implementation of RingBuffer that uses the POSIX trick and a unit test for it.
   Add a performance comparison test to this unit test that compares the two versions by fuzzing them with random data and random read/write operations. Make sure that you setup this fuzzing so that the same operations are done to each so you can compare them between runs.
   Use callgrind and cachegrind to compare the performance of these two.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 


















