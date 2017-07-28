Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 38: Hashmap Algorithms

There are three hash functions that you'll implement in this exercise:

FNV-1a
    Named after the creators Glenn Fowler, Phong Vo, and Landon Curt Noll. This hash produces good numbers and is reasonably fast.
Adler-32
    Named after Mark Adler, is a horrible hash algorithm, but it's been around a long time and it's good for studying.
DJB Hash
    This hash algorithm is attributed to Dan J. Bernstein (DJB) but it's difficult to find his discussion of the algorithm. It's shown to be fast, but possibly not great numbers.

You've already seen the Jenkins hash as the default hash for the Hashmap data structure, so this exercise will be looking at these three new ones. The code for them is usually small, and it's not optimized at all. As usual I'm going for understanding and not blinding latest speed.

The header file is very simple, so I'll start with that:
    
    #ifndef hashmap_algos_h
    #define hashmap_algos_h
    
    #include <stdint.h>
    
    uint32_t Hashmap_fnv1a_hash(void *data);
    
    uint32_t Hashmap_adler32_hash(void *data);
    
    uint32_t Hashmap_djb_hash(void *data);
    
    #endif
    

I'm just declaring the three functions I'll implement in the hashmap_algos.c file:
    
    #include <lcthw/hashmap_algos.h>
    #include <lcthw/bstrlib.h>
    
    // settings taken from
    // http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
    const uint32_t FNV_PRIME = 16777619;
    const uint32_t FNV_OFFSET_BASIS = 2166136261;
    
    uint32_t Hashmap_fnv1a_hash(void *data)
    {
        bstring s = (bstring)data;
        uint32_t hash = FNV_OFFSET_BASIS;
        int i = 0;
    
        for(i = 0; i < blength(s); i++) {
            hash ^= bchare(s, i, 0);
            hash *= FNV_PRIME;
        }
    
        return hash;
    }
    
    const int MOD_ADLER = 65521;
    
    uint32_t Hashmap_adler32_hash(void *data)
    {
        bstring s = (bstring)data;
        uint32_t a = 1, b = 0;
        int i = 0;
    
        for (i = 0; i < blength(s); i++)
        {
            a = (a + bchare(s, i, 0)) % MOD_ADLER;
            b = (b + a) % MOD_ADLER;
        }
    
        return (b << 16) | a;
    }
    
    uint32_t Hashmap_djb_hash(void *data)
    {
        bstring s = (bstring)data;
        uint32_t hash = 5381;
        int i = 0;
    
        for(i = 0; i < blength(s); i++) {
            hash = ((hash << 5) + hash) + bchare(s, i, 0); /* hash * 33 + c */
        }
    
        return hash;
    }
    

This file then has the three hash algorithms. You should notice that I'm defaulting to just using a bstring for the key, and I'm using the bchare function to get a character from the bstring, but return 0 if that character is outside the string's length.

Each of these algorithms are found online so go search for them and read about them. Again I used Wikipedia primarily and then followed it to other sources.

I then have a unit test that tests out each algorithm, but also tests that it will distribute well across a number of buckets:
    
    #include <lcthw/bstrlib.h>
    #include <lcthw/hashmap.h>
    #include <lcthw/hashmap_algos.h>
    #include <lcthw/darray.h>
    #include "minunit.h"
    
    struct tagbstring test1 = bsStatic("test data 1");
    struct tagbstring test2 = bsStatic("test data 2");
    struct tagbstring test3 = bsStatic("xest data 3");
    
    char *test_fnv1a()
    {
        uint32_t hash = Hashmap_fnv1a_hash(&test1);
        mu_assert(hash != 0, "Bad hash.");
    
        hash = Hashmap_fnv1a_hash(&test2);
        mu_assert(hash != 0, "Bad hash.");
    
        hash = Hashmap_fnv1a_hash(&test3);
        mu_assert(hash != 0, "Bad hash.");
    
        return NULL;
    }
    
    char *test_adler32()
    {
        uint32_t hash = Hashmap_adler32_hash(&test1);
        mu_assert(hash != 0, "Bad hash.");
    
        hash = Hashmap_adler32_hash(&test2);
        mu_assert(hash != 0, "Bad hash.");
    
        hash = Hashmap_adler32_hash(&test3);
        mu_assert(hash != 0, "Bad hash.");
    
        return NULL;
    }
    
    char *test_djb()
    {
        uint32_t hash = Hashmap_djb_hash(&test1);
        mu_assert(hash != 0, "Bad hash.");
    
        hash = Hashmap_djb_hash(&test2);
        mu_assert(hash != 0, "Bad hash.");
    
        hash = Hashmap_djb_hash(&test3);
        mu_assert(hash != 0, "Bad hash.");
    
        return NULL;
    }
    
    #define BUCKETS 100
    #define BUFFER_LEN 20
    #define NUM_KEYS BUCKETS * 1000
    enum { ALGO_FNV1A, ALGO_ADLER32, ALGO_DJB};
    
    int gen_keys(DArray *keys, int num_keys)
    {
        int i = 0;
        FILE *urand = fopen("/dev/urandom", "r");
        check(urand != NULL, "Failed to open /dev/urandom");
    
        struct bStream *stream = bsopen((bNread)fread, urand);
        check(stream != NULL, "Failed to open /dev/urandom");
    
        bstring key = bfromcstr("");
        int rc = 0;
    
        // FNV1a histogram
        for(i = 0; i < num_keys; i++) {
            rc = bsread(key, stream, BUFFER_LEN);
            check(rc >= 0, "Failed to read from /dev/urandom.");
    
            DArray_push(keys, bstrcpy(key));
        }
    
        bsclose(stream);
        fclose(urand);
        return 0;
    
    error:
        return -1;
    }
    
    void destroy_keys(DArray *keys)
    {
        int i = 0;
        for(i = 0; i < NUM_KEYS; i++) {
            bdestroy(DArray_get(keys, i));
        }
    
        DArray_destroy(keys);
    }
    
    void fill_distribution(int *stats, DArray *keys, Hashmap_hash hash_func)
    {
        int i = 0;
        uint32_t hash = 0;
    
        for(i = 0; i < DArray_count(keys); i++) {
            hash = hash_func(DArray_get(keys, i));
            stats[hash % BUCKETS] += 1;
        }
    
    }
    
    char *test_distribution()
    {
        int i = 0;
        int stats[3][BUCKETS] = {{0}};
        DArray *keys = DArray_create(0, NUM_KEYS);
    
        mu_assert(gen_keys(keys, NUM_KEYS) == 0, "Failed to generate random keys.");
    
        fill_distribution(stats[ALGO_FNV1A], keys, Hashmap_fnv1a_hash);
        fill_distribution(stats[ALGO_ADLER32], keys, Hashmap_adler32_hash);
        fill_distribution(stats[ALGO_DJB], keys, Hashmap_djb_hash);
    
        fprintf(stderr, "FNV\tA32\tDJB\n");
    
        for(i = 0; i < BUCKETS; i++) {
            fprintf(stderr, "%d\t%d\t%d\n",
                    stats[ALGO_FNV1A][i],
                    stats[ALGO_ADLER32][i],
                    stats[ALGO_DJB][i]);
        }
    
        destroy_keys(keys);
    
        return NULL;
    }
    
    char *all_tests()
    {
        mu_suite_start();
    
        mu_run_test(test_fnv1a);
        mu_run_test(test_adler32);
        mu_run_test(test_djb);
        mu_run_test(test_distribution);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

I have the number of BUCKETS in this code set fairly high, since I have a fast enough computer, but if it runs slow just lower them, and also lower NUM_KEYS. What this test lets me do is run the test and then look at the distribution of keys for each hash function using a bit of analysis with a language called R.

How I do this is I craft a big list of keys using the gen_keys function. These keys are taken out of the /dev/urandom device they are random byte keys. I then use these keys to have the fill_distribution function fill up the stats array with where those keys would hash in a theoretial set of buckets. All this function does is go through all the keys, do the hash, then do what the Hashmap would do to find its bucket.

Finally I'm simply printing out a three column table of the final count for each bucket, showing how many keys managed to get into each bucket randomly. I can then look at these numbers to see if the hash functions are distributing keys mostly evenly.

# What You Should See

Teaching you R is outside the scope of this book, but if you want to get it and try this then it can be found at r-project.org.

Here is an abbreviated shell session showing me run the tests/hashmap_algos_test to get the table produced by test_distribution (not shown here), and then use R to see what the summary statistics are:
    
    $ tests/hashmap_algos_tests
    # copy-paste the table it prints out
    $ vim hash.txt
    $ R
    > hash <- read.table("hash.txt", header=T)
    > summary(hash)
          FNV            A32              DJB      
     Min.   : 945   Min.   : 908.0   Min.   : 927  
     1st Qu.: 980   1st Qu.: 980.8   1st Qu.: 979  
     Median : 998   Median :1000.0   Median : 998  
     Mean   :1000   Mean   :1000.0   Mean   :1000  
     3rd Qu.:1016   3rd Qu.:1019.2   3rd Qu.:1021  
     Max.   :1072   Max.   :1075.0   Max.   :1082  
    >
    

First I just run the test, which on your screen will print the table. Then I just copy-paste it out of my terminal and use vim hash.txt to save the data. If you look at the data it has the header FNV A32 DJB for each of the three algorithms.

Second I run R and load the data using the read.table command. This is a smart function that works with this kind of tab-delimited data and I only have to tell it header=T so it knows the data has a header.

Finally, I have the data loaded and I can use summary to print out its summary statistics for each column. Here you can see that each function actually does alright with this random data. I'll explain what each of these rows means:

Min.
    This is the minimum value found for the data in that column. FNV seems to win this on this run since it has the largest number, meaning it has a tighter range at the low end.
1st Qu.
    The point where the first quarter of the data ends.
Median
    This is the number that is in the middle if you sorted them. Median is most useful when compared to mean.
Mean
    Mean is the "average" most people think of, and is the sum/count of the data. If you look, all of them are 1000, which is great. If you compare this to the median you see that all three have really close medians to the mean. What this means is the data isn't "skewed" in one direction, so you can trust the mean.
3rd Qu.
    The point where the last quarter of the data starts and represents the tail end of the numbers.
Max.
    This is the maximum number of the data, and presents the upper bound on all of them.

Looking at this data, you see that all of these hashes seem to do good on random keys, and that the means match the NUM_KEYS setting I made. What I'm looking for is that if I make 1000 keys per buckets (BUCKETS * 1000), then on average each bucket should have 1000 keys in it. If the hash function isn't working then you'll see these summary statistics show a Mean that's not 1000, and really high ranges at the 1st quarter and 3rd quarter. A good hash function should have a dead on 1000 mean, and as tight as possible range.

You should also know that you will get mostly different numbers from mine, and even between different runs of this unit test.

# How To Break It

I'm finally going to have you do some breaking in this exercise. I want you to write the worst hash function you can, and then use the data to prove that it's really bad. You can use R to do the stats, just like I did, but maybe you have another tool you can use to give you the same summary statistics.

The goal is to make a hash function that seems normal to an untrained eye, but when actually run has a bad mean and is all over the place. That means you can't just have it return 1, but have to give a stream of numbers that seem alright, but really are all over the place and loading up some buckets too much.

Extra points if you can make a minimal change to one of the four hash algorithms I gave you to do this.

The purpose of this exercise is to imagine that some "friendly" coder comes to you and offers to improve your hash function, but actually just makes a nice little backdoor that screws up your Hashmap.

As the Royal Society says, "Nullius in verba."

# Extra Credit

   Take the default_hash out of the hashmap.c, make it one of the algorithms in hashmap_algos.c and then make all the tests work again.
   Add the default_hash to the hashmap_algos_tests.c test and compare its statistics to the other hash functions.
   Find a few more hash functions and add them too. You can never have too many hash functions!

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 


















