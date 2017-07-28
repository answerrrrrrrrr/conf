#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

int main()
{
    int status, i;

    regex_t reg;
    const char *pattern = "^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*.\\w+([-.]\\w+)*$";
    int cflags = REG_EXTENDED;

    char *string = "david19842gmail.com";
    const size_t nmatch = 1;
    regmatch_t pmatch[1];

    regcomp(&reg, pattern, cflags);

    status = regexec(&reg, string, nmatch, pmatch, 0);
    
    if(status == 1){
        printf("No match\n");
    }
    else if(status == 0){
        printf("Match\n");
        for(i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++){
            putchar(string[i]);
        }
    }
}
