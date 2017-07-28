Learn C The Hard Way

Online Video Course Plus PDFs $29

Python | Ruby | C | SQL | Regex

# Exercise 47: A Fast URL Router

I'm going to now show you how I use the TSTree to do fast URL routing in web servers I've written. This works for simple URL routing you might use at the edge of an application, not really for the more complex (and sometimes unecessary) routing found in many web application frameworks.

To play with routing I'm going to make a little command line tool I'm calling urlor that reads a simple file of routes, and then prompts the user to enter in URLs to look up.
    
    #include <lcthw/tstree.h>
    #include <lcthw/bstrlib.h>
    
    TSTree *add_route_data(TSTree *routes, bstring line)
    {
        struct bstrList *data = bsplit(line, ' ');
        check(data->qty == 2, "Line '%s' does not have 2 columns",
                bdata(line));
    
        routes = TSTree_insert(routes,
                bdata(data->entry[0]), blength(data->entry[0]),
                bstrcpy(data->entry[1]));
    
        bstrListDestroy(data);
    
        return routes;
    
    error:
        return NULL;
    }
    
    TSTree *load_routes(const char *file)
    {
        TSTree *routes = NULL;
        bstring line = NULL;
        FILE *routes_map = NULL;
    
        routes_map = fopen(file, "r");
        check(routes_map != NULL, "Failed to open routes: %s", file);
    
        while((line = bgets((bNgetc)fgetc, routes_map, '\n')) != NULL) {
            check(btrimws(line) == BSTR_OK, "Failed to trim line.");
            routes = add_route_data(routes, line);
            check(routes != NULL, "Failed to add route.");
            bdestroy(line);
        }
    
        fclose(routes_map);
        return routes;
    
    error:
        if(routes_map) fclose(routes_map);
        if(line) bdestroy(line);
    
        return NULL;
    }
    
    bstring match_url(TSTree *routes, bstring url)
    {
        bstring route = TSTree_search(routes, bdata(url), blength(url));
    
        if(route == NULL) {
            printf("No exact match found, trying prefix.\n");
            route = TSTree_search_prefix(routes, bdata(url), blength(url));
        }
    
        return route;
    }
    
    bstring read_line(const char *prompt)
    {
        printf("%s", prompt);
    
        bstring result = bgets((bNgetc)fgetc, stdin, '\n');
        check_debug(result != NULL, "stdin closed.");
    
        check(btrimws(result) == BSTR_OK, "Failed to trim.");
    
        return result;
    
    error:
        return NULL;
    }
    
    void bdestroy_cb(void *value, void *ignored)
    {
        (void)ignored;
        bdestroy((bstring)value);
    }
    
    void destroy_routes(TSTree *routes)
    {
        TSTree_traverse(routes, bdestroy_cb, NULL);
        TSTree_destroy(routes);
    }
    
    int main(int argc, char *argv[])
    {
        bstring url = NULL;
        bstring route = NULL;
        check(argc == 2, "USAGE: urlor <urlfile>");
    
        TSTree *routes = load_routes(argv[1]);
        check(routes != NULL, "Your route file has an error.");
    
        while(1) {
            url = read_line("URL> ");
            check_debug(url != NULL, "goodbye.");
    
            route = match_url(routes, url);
    
            if(route) {
                printf("MATCH: %s == %s\n", bdata(url), bdata(route));
            } else {
                printf("FAIL: %s\n", bdata(url));
            }
    
            bdestroy(url);
        }
    
        destroy_routes(routes);
        return 0;
    
    error:
        destroy_routes(routes);
        return 1;
    }
    

I'll then make a simple file with some fake routes to play with:

/ MainApp /hello Hello /hello/ Hello /signup Signup /logout Logout /album/ Album

# What You Should See

Once you have urlor working and a routes file, you can try it out:
    
    $ ./bin/urlor urls.txt
    URL> /
    MATCH: / == MainApp
    URL> /hello
    MATCH: /hello == Hello
    URL> /hello/zed  
    No exact match found, trying prefix.
    MATCH: /hello/zed == Hello
    URL> /album
    No exact match found, trying prefix.
    MATCH: /album == Album
    URL> /album/12345
    No exact match found, trying prefix.
    MATCH: /album/12345 == Album
    URL> asdfasfdasfd
    No exact match found, trying prefix.
    FAIL: asdfasfdasfd
    URL> /asdfasdfasf
    No exact match found, trying prefix.
    MATCH: /asdfasdfasf == MainApp
    URL>
    $
    

You can see that the routing system first tries an exact match, and then if it can't find one it will give a prefix match. This is mostly to try out the difference between the two. Depending on the semantics of your URLs you may want to always match exactly, always to prefixes, or do both and pick the "best" one.

# How To Improve It

URLs are weird because people want them to magically handle all of the insane things their web applications do, even if that's not very logical. In this simple demonstration of how to use the TSTree to do routing, it has some flaws that people wouldn't be able to articulate. For example, it will match /al to Album, which generall isn't what they want. They want /album/* to match Album and /al to be a 404 error.

This isn't difficult to implement though, since you could change the prefix algorithm to match any way you want. If you change the matching algorithm to find all matching prefixes, and then pick the "best" one, you'll be able to do it easily. In this case, /al could match MainApp or Album. Take those results then do a little logic on which is "best".

Another thing you can do in a real routing system is use the TSTree to finall possible matches, but that these matches are a small set of patterns to check. In many web applications there's a list of regex that have to be matched against URLs on each request. Running all the regex can be time consuming, so you can use a TSTree to find all the possible ones by their prefixes. Then you narrow the patterns to try down to a few very quickly.

Using this method, your URLs will match exactly since you are actually running real regex patterns, and they'll match much faster since you're finding them by possible prefixes.

This kind of algorithm also works for anything else that needs to have flexible user-visible routing mechanisms. Domain names, IP address, registries and directories, files, or URLs.

# Extra Credit

   Instead of just storing the string for the handler, create an actual engine that uses an Handler struct to store the application. The struct would store the URL it is attached to, the name, and anything else you'd need to make an actual routing system.
   Instead of mapping URLs to arbitrary names, map them to .so files and use the dlopen system to load handlers on the fly and call callbacks they contain. Put these callbacks in your Handler struct and then you have yourself a fully dynamic callback handler system in C.

Donate

Copyright (C) 2010 Zed. A. Shaw

Credits

 

















