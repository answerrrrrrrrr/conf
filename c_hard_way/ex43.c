Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 43: A Simple Statistics Engine

This is a simple algorithm I use for collecting summary statistics "online", or without storing all of the samples. I use this in any software that needs to keep some statistics such as mean, standard deviation, and sum, but where I can't store all the samples needed. Instead I can just store the rolling results of the calculations which is only 5 numbers.

# Rolling Standard Deviation And Mean

The first thing you need is a sequence of samples. This can be anything from time to complete a task, numbers of times someone accesses something, or even star ratings on a website. Doesn't really matter what, just so long as you have a stream of numbers and you want to know the following summary statistics about them:

sum
    This is the total of all the numbers added together.
sum squared (sumsq)
    This is the sum of the square of each number.
count (n)
    This is the number samples you've taken.
min
    This is the smallest sample you've seen.
max
    This is the largest sample you've seen.
mean
    This is the most likely middle number. It's not actually the middle, since that's the median, but it's an accepted approximation for it.
stddev
    Calculated using $sqrt(sumsq - (sum * mean)) / (n - 1) ))$ where sqrt is the square root function in the math.h header.

I will confirm this calculation works using R since I know R gets these right:
    
    > s <- runif(n=10, max=10)
    > s
     [1] 6.1061334 9.6783204 1.2747090 8.2395131 0.3333483 6.9755066 1.0626275
     [8] 7.6587523 4.9382973 9.5788115
    > summary(s)
       Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
     0.3333  2.1910  6.5410  5.5850  8.0940  9.6780 
    > sd(s)
    [1] 3.547868
    > sum(s)
    [1] 55.84602
    > sum(s * s)
    [1] 425.1641
    > sum(s) * mean(s)
    [1] 311.8778
    > sum(s * s) - sum(s) * mean(s)
    [1] 113.2863
    > (sum(s * s) - sum(s) * mean(s)) / (length(s) - 1)
    [1] 12.58737
    > sqrt((sum(s * s) - sum(s) * mean(s)) / (length(s) - 1))
    [1] 3.547868
    >
    

You don't need to know R, just follow along while I explain how I'm breaking this down to check my math:

lines 1-4
    I use the function runif to get a "random uniform" distribution of numbers, then print them out. I'll use these in the unit test later.
lines 5-7
    Here's the summary, so you can see the values that R calculates for these.
lines 8-9
    This is the stddev using the sd function.
lines 10-11
    

Now I begin to build this calculation manually, first by getting the

sum.

lines 12-13
    Next piece of the stdev formula is the sumsq, which I can get with sum(s * s) which tells R to multiple the whole s list by itself and then sum those.footnote{The power of R is being able to do math on entire data structures like this.}
lines 14-15
    Looking at the formula, I then need the sum multiplied by mean, so I do sum(s) * mean(s).
lines 16-17
    I then combine the sumsq with this to get sum(s * s) - sum(s) * mean(s).
lines 18-19
    That needs to be divided by $n-1$, so I do (sum(s * s) - sum(s) * mean(s)) / (length(s) - 1).
lines 20-21
    Finally, I sqrt that and I get 3.547868 which matches the number R gave me for sd above.

# Implemention

That's how you calculate the stddev, so now I can make some simple code to implement this calculation.
    
    #ifndef lcthw_stats_h
    #define lctwh_stats_h
    
    typedef struct Stats {
        double sum;
        double sumsq;
        unsigned long n;
        double min;
        double max;
    } Stats;
    
    Stats *Stats_recreate(double sum, double sumsq, unsigned long n, double min, double max);
    
    Stats *Stats_create();
    
    double Stats_mean(Stats *st);
    
    double Stats_stddev(Stats *st);
    
    void Stats_sample(Stats *st, double s);
    
    void Stats_dump(Stats *st);
    
    #endif
    

Here you can see I've put the calculations I need to store in a struct and then I have functions for sampling and getting the numbers. Implementing this is then just an exercise in converting the math:
    
    #include <math.h>
    #include <lcthw/stats.h>
    #include <stdlib.h>
    #include <lcthw/dbg.h>
    
    Stats *Stats_recreate(double sum, double sumsq, unsigned long n, double min, double max)
    {
        Stats *st = malloc(sizeof(Stats));
        check_mem(st);
    
        st->sum = sum;
        st->sumsq = sumsq;
        st->n = n;
        st->min = min;
        st->max = max;
    
        return st;
    
    error:
        return NULL;
    }
    
    Stats *Stats_create()
    {
        return Stats_recreate(0.0, 0.0, 0L, 0.0, 0.0);
    }
    
    double Stats_mean(Stats *st)
    {
        return st->sum / st->n;
    }
    
    double Stats_stddev(Stats *st)
    {
       return sqrt( (st->sumsq - ( st->sum * st->sum / st->n)) / (st->n - 1) );
    }
    
    void Stats_sample(Stats *st, double s)
    {
        st->sum += s;
        st->sumsq += s * s;
    
        if(st->n == 0) {
            st->min = s;
            st->max = s;
        } else {
            if(st->min > s) st->min = s;
            if(st->max < s) st->max = s;
        }
    
        st->n += 1;
    }
    
    void Stats_dump(Stats *st)
    {
        fprintf(stderr, "sum: %f, sumsq: %f, n: %ld, min: %f, max: %f, mean: %f, stddev: %f",
                st->sum, st->sumsq, st->n, st->min, st->max,
                Stats_mean(st), Stats_stddev(st));
    }
    

Here's what each function in stats.c does:

Stats_recreate
    I'll want to load these numbers from some kind of database, and this function let's me recreate a Stats struct.
Stats_create
    Simply called Stats_recreate with all 0 values.
Stats_mean
    Using the sum and n it gives the mean.
Stats_stddev
    Implements the formula I worked out, with the only difference being that I calculate the mean with st->sum / st->n in this formula instead of calling Stats_mean.
Stats_sample
    This does the work of maintaining the numbers in the Stats struct. When you give it the first value it sees that n is 0 and sets min and max accordingly. Every call after that keeps increasing sum, sumsq, and n. It then figures out if this new sample is a new min or max.
Stats_dump
    Simple debug function that dumps the stats so you can view them.

The last thing I need to do is confirm that this math is correct. I'm going to use my numbers and calculations from my R session to create a unit test that confirms I'm getting the right results.
    
    #include "minunit.h"
    #include <lcthw/stats.h>
    #include <math.h>
    
    const int NUM_SAMPLES = 10;
    double samples[] = {
        6.1061334, 9.6783204, 1.2747090, 8.2395131, 0.3333483,
        6.9755066, 1.0626275, 7.6587523, 4.9382973, 9.5788115
    };
    
    Stats expect = {
        .sumsq = 425.1641,
        .sum = 55.84602,
        .min = 0.333,
        .max = 9.678,
        .n = 10,
    };
    double expect_mean = 5.584602;
    double expect_stddev = 3.547868;
    
    #define EQ(X,Y,N) (round((X) * pow(10, N)) == round((Y) * pow(10, N)))
    
    char *test_operations()
    {
        int i = 0;
        Stats *st = Stats_create();
        mu_assert(st != NULL, "Failed to create stats.");
    
        for(i = 0; i < NUM_SAMPLES; i++) {
            Stats_sample(st, samples[i]);
        }
    
        Stats_dump(st);
    
        mu_assert(EQ(st->sumsq, expect.sumsq, 3), "sumsq not valid");
        mu_assert(EQ(st->sum, expect.sum, 3), "sum not valid");
        mu_assert(EQ(st->min, expect.min, 3), "min not valid");
        mu_assert(EQ(st->max, expect.max, 3), "max not valid");
        mu_assert(EQ(st->n, expect.n, 3), "max not valid");
        mu_assert(EQ(expect_mean, Stats_mean(st), 3), "mean not valid");
        mu_assert(EQ(expect_stddev, Stats_stddev(st), 3), "stddev not valid");
    
        return NULL;
    }
    
    char *test_recreate()
    {
        Stats *st = Stats_recreate(expect.sum, expect.sumsq, expect.n, expect.min, expect.max);
    
        mu_assert(st->sum == expect.sum, "sum not equal");
        mu_assert(st->sumsq == expect.sumsq, "sumsq not equal");
        mu_assert(st->n == expect.n, "n not equal");
        mu_assert(st->min == expect.min, "min not equal");
        mu_assert(st->max == expect.max, "max not equal");
        mu_assert(EQ(expect_mean, Stats_mean(st), 3), "mean not valid");
        mu_assert(EQ(expect_stddev, Stats_stddev(st), 3), "stddev not valid");
    
        return NULL;
    }
    
    char *all_tests()
    {
        mu_suite_start();
    
        mu_run_test(test_operations);
        mu_run_test(test_recreate);
    
        return NULL;
    }
    
    RUN_TESTS(all_tests);
    

There's nothing new in this unit test, except maybe the EQ macro. I felt lazy and didn't want to look up the standard way to tell if two double values are close, so I made this macro. The problem with double is that equality assumes totally equal, but I'm using two different systems with slightly different rounding errors. The solution is to say I want the numbers to be "equal to X decimal places".

I do this with EQ by raising the number to a power of 10, then using the round function to get an integer. This is a simple way to round to N decimal places and compare the results as an integer. I'm sure there's a billion other ways to do the same thing, but this works for now.

The expected results are then in a Stats struct and then I simply make sure that the number I get is close to the number R gave me.

# How To Use It

You can use the standard deviation and mean to determine if a new sample is "interesting", or you can use this to collect statistics on statistics. The first one is easy for people to understand so I'll explain that quickly using an example for login times.

Imagine you're tracking how long users spend on a server and you're using stats to analyze it. Every time someone logs in, you keep track of how long they are there, then you call Stats_sample. I'm looking for people are a on "too long" and also people who seem to be on "too quickly".

Instead of setting specific levels, what I'd do is compare how long someone is on with the mean (plus or minus) 2 * stddev range. I get the mean and 2 * stddev, and consider login times to be "interesting" if they are outside these two ranges. Since I'm keeping these statistics using a rolling algorithm this is a very fast calculation and I can then have the software flag the users who are outside of this range.

This doesn't necessarily point out people who are behaving badly, but instead it flags potential problems that you can review to see what's going on. It's also doing it based on the behavior of all the users, which avoids the problem where you pick some arbitrary number that's not based on what's really happening.

The general rule you can get from this is that the mean (plus or minus) 2 * stddev is an estimate of where 90% of the values are expected to fall, and that anything outside those ranges is interesting.

The second way to use these statistics is to go meta and calculate them for other Stats calculations. You basically do your Stats_sample like normal, but then you run Stats_sample on the min, max, n, mean, and stddev on that sample. This gives a two-level measurement, and let's you compare samples of samples.

Confusing right? I'll continue my example above and add that you have 100 servers that each hold a different application. You are already tracking user's login times for each application server, but you want to compare all 100 applications and flag any users that are logging in "too much" on all of them. Easiest way to do that is each time someone logs in, calculate the new login stats, then add that Stats structs elements to a second Stat.

What you end up with is a series of stats that can be named like this:

mean of means
    This is a full Stats struct that gives you mean and stddev of the means of all the servers. Any server or user who is outside of this is work looking at on a global level.
mean of stddevs
    Another Stats struct that produces the statistics of how all of the servers range. You can then analyze each server and see if any of them have unusually wide ranging numbers by comparing their stddev to this mean of stddevs statistic.

You could do them all, but these are the most useful. If you wanted to then monitor servers for erratic login times you'd do this:

   User John logs into and out of server A. Grab server A's stats, update them.
   Grab the mean of means stats, and take A's mean and add it as a sample. I'll call this m_of_m.
   Grab the mean of stddevs stats, and add A's stddev to it as a sample. I'll call this m_of_s.
   If A's mean is outside of m_of_m.mean + 2 * m_of_m.stddev then flag it as possibly having a problem.
   If A's stddev is outside of m_of_s.mean + 2 * m_of_s.stddev then flag it as possible behaving too erratically.
   Finally, if John's login time is outside of A's range, or A's m_of_m range, then flag it as interesting.

Using this "mean of means" and "mean of stddevs" calculation you can do efficient tracking of many metrics with a minimal amount of processing and storage.

# Extra Credit

   Convert the Stats_stddev and Stats_mean to static inline functions in the stats.h file instead of in the stats.c file.
   Use this code to write a performance test of the string_algos_test.c. Make it optional and have it run the base test as a series of samples then report the results.
   Write a version of this in another programming language you know. Confirm that this version is correct based on what I have here.
   Write a little program that can take a file full of numbers and spit these statistics out for them.
   Make the program accept a table of data that has headers on one line, then all the other numbers on lines after it separated by any number of spaces. Your program should then print out these stats for each column by the header name.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















