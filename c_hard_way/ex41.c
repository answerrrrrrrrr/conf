Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 41: Using Cachegrind And Callgrind For Performance Tuning

In this exercise I'm going to give you a quick course in using two tools for Valgrind called callgrind and cachegrind. These two tools will analyze your program's execution and tell you what parts are running slow. The results are accurate because of the way Valgrind works and help you spot problems such as lines of code that execute too much, hot spots, memory access problems, and even CPU cache misses.

To do this exercise I'm going to use the bstree_tests unit tests you just did to look for places to improve the algorithms used. Make sure your versions of these programs are running without any valgrind errors and that it is exactly like my code. I'll be using dumps of my code to talk about how cachegrind and callgrind work.

# Running Callgrind

To run callgrind you pass the --tool=callgrind option to valgrind and it will produce a callgrind.out.PID file (where PID is replace with the process ID of the program that ran). Once you run it you can analyze this callgrind.out file with a tool called callgrind_annotate which will tell you which functions used the most instructions to run. Here's an example of me running callgrind on bstree_tests and then getting its information:
    
    $ valgrind --dsymutil=yes --tool=callgrind tests/bstree_tests
    ...
    $ callgrind_annotate callgrind.out.1232
    --------------------------------------------------------------------------------
    Profile data file 'callgrind.out.1232' (creator: callgrind-3.7.0.SVN)
    --------------------------------------------------------------------------------
    I1 cache: 
    D1 cache: 
    LL cache: 
    Timerange: Basic block 0 - 1098689
    Trigger: Program termination
    Profiled target:  tests/bstree_tests (PID 1232, part 1)
    Events recorded:  Ir
    Events shown:     Ir
    Event sort order: Ir
    Thresholds:       99
    Include dirs:     
    User annotated:   
    Auto-annotation:  off
    
    --------------------------------------------------------------------------------
           Ir 
    --------------------------------------------------------------------------------
    4,605,808  PROGRAM TOTALS
    
    --------------------------------------------------------------------------------
           Ir  file:function
    --------------------------------------------------------------------------------
      670,486  src/lcthw/bstrlib.c:bstrcmp [tests/bstree_tests]
      194,377  src/lcthw/bstree.c:BSTree_get [tests/bstree_tests]
       65,580  src/lcthw/bstree.c:default_compare [tests/bstree_tests]
       16,338  src/lcthw/bstree.c:BSTree_delete [tests/bstree_tests]
       13,000  src/lcthw/bstrlib.c:bformat [tests/bstree_tests]
       11,000  src/lcthw/bstrlib.c:bfromcstralloc [tests/bstree_tests]
        7,774  src/lcthw/bstree.c:BSTree_set [tests/bstree_tests]
        5,800  src/lcthw/bstrlib.c:bdestroy [tests/bstree_tests]
        2,323  src/lcthw/bstree.c:BSTreeNode_create [tests/bstree_tests]
        1,183  /private/tmp/pkg-build/coregrind//vg_preloaded.c:vg_cleanup_env [/usr/local/lib/valgrind/vgpreload_core-amd64-darwin.so]
    
    $
    

I've removed the unit test run and the valgrind output since it's not very useful for this exercise. What you should look at is the callgrind_anotate output. What this shows you is the number of instructions run (which valgrind calls Ir) for each function, and the functions sorted highest to lowest. You can usually ignore the header data and just jump to the list of functions.

Note

In if you get a ton of "???:Image" lines and things that are not in your program then you're picking up junk from the OS. Just add | grep -v "???" at the end to filter those out, like this.

I can now do a quick breakdown of this output to figure out where to look next:

   Each line lists the number of Ir and the file:function that executed them. The Ir is just the instruction count, and if you make that lower then you have made it faster. There's some complexity to that, but at first just focus on getting the Ir down.
   The way to attack this is to look at your top functions, and then see which one you think you can improve first. In this case, I'd look at improving bstrcmp or BStree_get. It's probably easier to start with BStree_get.
   Some of these functions are just called from the unit test, so I would just ignore those. Functions like bformat, bfromcstralloc, and bdestroy fit this description.
   I would also look for functions I can simply avoid calling. For example, maybe I can just say BSTree only works with bstring keys, and then I can just not use the callback system and remove default_compare entirely.

At this point though, I only know that I want to look at BSTree_get to improve it, and not the reason BSTree_get is slow. That is phase two of the analysis.

# Callgrind Annotating Source

I will next tell callgrind_annotate to dump out the bstree.c file and annotate each line with the number of Ir it took. You get the annotated source file by running:
    
    $ callgrind_annotate callgrind.out.1232 src/lcthw/bstree.c
    ...
    

Your output will have a big dump of the file's source, but I've cut out the parts for BSTree_get and BSTree_getnode:
    
    
    --------------------------------------------------------------------------------
    -- User-annotated source: src/lcthw/bstree.c
    --------------------------------------------------------------------------------
        Ir
    
    
     2,453  static inline BSTreeNode *BSTree_getnode(BSTree *map, BSTreeNode *node, void *key)
         .  {
    61,853      int cmp = map->compare(node->key, key);
    663,908  => src/lcthw/bstree.c:default_compare (14850x)
         .
    14,850      if(cmp == 0) {
         .          return node;
    24,794      } else if(cmp < 0) {
    30,623          if(node->left) {
         .              return BSTree_getnode(map, node->left, key);
         .          } else {
         .              return NULL;
         .          }
         .      } else {
    13,146          if(node->right) {
         .              return BSTree_getnode(map, node->right, key);
         .          } else {
         .              return NULL;
         .          }
         .      }
         .  }
         .
         .  void *BSTree_get(BSTree *map, void *key)
     4,912  {
    24,557      if(map->root == NULL) {
    14,736          return NULL;
         .      } else {
         .          BSTreeNode *node = BSTree_getnode(map, map->root, key);
     2,453          return node == NULL ? NULL : node->data;
         .      }
         .  }
    

Each line is shown with either the number of Ir (instructions) it ran, or a period (.) to show that it's not important. What I'm looking for is hotspots, or lines that have huge numbers of Ir that I can possibly bring down. In this case, line 10 of the output above shows that what makes BSTree_getnode so expensive is that it calls default_comapre which calls bstrcmp. I already know that bstrcmp is the worst running function, so if I want to improve the speed of BSTree_getnode I should work on that first.

I'll then look at bstrcmp the same way:
    
    
     98,370  int bstrcmp (const_bstring b0, const_bstring b1) {
          .  int i, v, n;
          .
    196,740     if (b0 == NULL || b1 == NULL || b0->data == NULL || b1->data == NULL ||
     32,790             b0->slen < 0 || b1->slen < 0) return SHRT_MIN;
     65,580     n = b0->slen; if (n > b1->slen) n = b1->slen;
     89,449     if (b0->slen == b1->slen && (b0->data == b1->data || b0->slen == 0))
          .             return BSTR_OK;
          .
     23,915     for (i = 0; i < n; i ++) {
    163,642             v = ((char) b0->data[i]) - ((char) b1->data[i]);
          .             if (v != 0) return v;
          .             if (b0->data[i] == (unsigned char) '\0') return BSTR_OK;
          .     }
          .
          .     if (b0->slen > n) return 1;
          .     if (b1->slen > n) return -1;
          .     return BSTR_OK;
          .  }
    

The Ir for this function shows two lines that take up most of the execution. First, bstrcmp seems to go through a lot of trouble to make sure that it is not given a NULL value. That's a good thing so I want to leave that alone, but I'd consider writing a different compare function that was more "risky" and assumed it was never given a NULL. The next one is the loop that does the actual comparison. It seems that there's some optimization that could be done in comparing the characters of the two data buffers.

# Analyzing Memory Access With Cachegrind

What I want to do next is see how many times this bstrcmp function access memory to either read it or write it. The tool for doing that (and other things) is cachegrind and you use it like this:
    
    $ valgrind --tool=cachegrind tests/bstree_tests
    ...
    $ cg_annotate --show=Dr,Dw cachegrind.out.1316 | grep -v "???"
    --------------------------------------------------------------------------------
    I1 cache:         32768 B, 64 B, 8-way associative
    D1 cache:         32768 B, 64 B, 8-way associative
    LL cache:         4194304 B, 64 B, 16-way associative
    Command:          tests/bstree_tests
    Data file:        cachegrind.out.1316
    Events recorded:  Ir I1mr ILmr Dr D1mr DLmr Dw D1mw DLmw
    Events shown:     Dr Dw
    Event sort order: Ir I1mr ILmr Dr D1mr DLmr Dw D1mw DLmw
    Thresholds:       0.1 100 100 100 100 100 100 100 100
    Include dirs:     
    User annotated:   
    Auto-annotation:  off
    
    --------------------------------------------------------------------------------
         Dr      Dw 
    --------------------------------------------------------------------------------
    997,124 349,058  PROGRAM TOTALS
    
    --------------------------------------------------------------------------------
         Dr     Dw  file:function
    --------------------------------------------------------------------------------
    169,754 19,430  src/lcthw/bstrlib.c:bstrcmp
     67,548 27,428  src/lcthw/bstree.c:BSTree_get
     19,430 19,430  src/lcthw/bstree.c:default_compare
      5,420  2,383  src/lcthw/bstree.c:BSTree_delete
      2,000  4,200  src/lcthw/bstrlib.c:bformat
      1,600  2,800  src/lcthw/bstrlib.c:bfromcstralloc
      2,770  1,410  src/lcthw/bstree.c:BSTree_set
      1,200  1,200  src/lcthw/bstrlib.c:bdestroy
    
    $
    

I tell valgrind to use the cachegrind tool, which runs bstree_tests and then produces a cachegrind.out.PID file just like callgrind did. I then use the program cg_annotate to get a similar output, but notice that I'm telling it to do --show=Dr,Dw. This option says that I only want the memory read Dr and write Dw counts for each function.

After that you get your usual header and then the counts for Dr and Dw for each file:function combination. I've edited this down so it shows the files and also removed any OS junk with | grep -v "???" so your output may be a little different. What you see in my output is that bstrcmp is the worst function for memory usage too, which is to be expected since that's mostly the only thing it does. I'm going to now dump it's annotated source to see where.
    
    
    --------------------------------------------------------------------------------
    -- User-annotated source: src/lcthw/bstrlib.c
    --------------------------------------------------------------------------------
        Dr     Dw
    
    
         0 19,430  int bstrcmp (const_bstring b0, const_bstring b1) {
         .      .  int i, v, n;
         .      .
    77,720      0       if (b0 == NULL || b1 == NULL || b0->data == NULL || b1->data == NULL ||
    38,860      0               b0->slen < 0 || b1->slen < 0) return SHRT_MIN;
         0      0       n = b0->slen; if (n > b1->slen) n = b1->slen;
         0      0       if (b0->slen == b1->slen && (b0->data == b1->data || b0->slen == 0))
         .      .               return BSTR_OK;
         .      .
         0      0       for (i = 0; i < n; i ++) {
    53,174      0               v = ((char) b0->data[i]) - ((char) b1->data[i]);
         .      .               if (v != 0) return v;
         .      .               if (b0->data[i] == (unsigned char) '\0') return BSTR_OK;
         .      .       }
         .      .
         .      .       if (b0->slen > n) return 1;
         .      .       if (b1->slen > n) return -1;
         .      .       return BSTR_OK;
         .      .  }
    

The surprising thing about this output is that the worst line of bstrcmp isn't the character comparison like I thought. For memory access it's that protective if-statement at the top that checks every possible bad variable it could receive. That one if statement does more than twice as many memory accesses compared to the line that's comparing the characters on line 17 of this output. If I were to make bstrcmp then I would definitely just ditch that or do it once somewhere else.

Another option is to turn this check into an assert that only exists when running in development, and then compile it out in production. I now have enough evidence to say that this line is bad for this data structure, so I can justify removing it.

What I don't want to do however is justify making this function less defensive to just gain a few more cycles. In a real performance improvement situation I would simply put this on a list and then dig for other gains I can make in the program.

# Judo Tuning

> "We should forget about small efficiencies, say about 97% of the time: premature optimization is the root of all evil."
> 
> --Donald Knuth

In my opinion, this quote seems to miss a major point about performance tuning. In this Knuth is saying that when you performance tune matters, in that if you do it in the beginning, then you'll cause all sorts of problems. According to him optimization should happen "sometime later", or at least that's my guess. Who knows these days really.

I'm going to declare this quote not necessarily wrong, but missing the point, and instead I'm going to officially give my quote. You can quote me on this:

> "Use evidence to find the biggest optimizations that take the least effort."
> 
> --Zed A. Shaw

It doesn't matter when you try to optimize something, but instead it's how you figure out if your optimization actually improved the software, and how much effort you put into doing them. With evidence you can find the places in the code where just a little effort gets you big improvements. Usually these places are just dumb decisions, as in bstrcmp trying to check everything possible for a NULL value.

At a certain point you have tuned the code to where the only thing that remains is tiny little micro-optimizations such as reorganizing if-statements and special loops like Duff's Device. At this point, just stop because there's a good chance that you'd gain more by redesigning the software to just not do things.

This is something that programmers who are optimizing simply fail to see. Many times the best way to do something fast is to find out ways to not do them. In the above analysis, I wouldn't try to make bstrcmp faster, I'd try to find a way to not use bstrcmp so much. Maybe there's a hashing scheme I can use that let's me do a sortable hash instead of constantly doing bstrcmp. Maybe I can optimize it by trying the first char first, and if it's comparable just don't call bstrcmp.

If after all that you can't do a redesign then start looking for little micro-optimizations, but as you do them constantly confirm they improve speed. Remember that the goal is to cause the biggest impact with the least effort possible.

# Using KCachegrind

The final section of this exercise is going to point you at a tool called KCachegrind is a fantastic GUI for analyzing callgrind and cachegrind output. I use it almost exclusively when I'm working on a Linux or BSD computer, and I've actually switched to just coding on Linux for projects because of KCachegrind.

Teaching you how to use it is outside the scope of this exercise, but you should be able to understand how to use it after this exercise. The output is nearly the same except KCachegrind lets you do the following:

   Graphically browse the source and execution times doing various sorts to find things to improve.
   Analyze different graphs to visually see what's taking up the most time and also what it is calling.
   Look at the actual machine code assembler output so you can see possible instructions that are happening, giving you more clues.
   Visualize the jump patterns for loops and branches in the source code, helping you find wayso to optimize the code easier.

You should spend some time getting KCachegrind installed and play with it.

# Extra Credit

   Read the callgrind manual and try some advanced options.
   Read the cachegrind manual and also try some advanced options.
   Use callgrind and cachegrind on all the unit tests and see if you can find optimizations to make. Did you find some things that surprised you? If not you probably aren't looking hard enough.
   Use KCachegrind and see how it compares to doing the terminal output like I'm doing here.
   Now use these tools to do the Exercise 40 extra credits and improvements.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 





















