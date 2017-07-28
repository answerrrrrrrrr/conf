#include <stdio.h>

int main()
{
    typedef struct{
        int a;
        char c;
    }A;
    A qwe;
    /*A qwe = {.a=1, .c=65};*/

    printf("%lx\n", &qwe.c);
    printf("%lx\n", qwe);
    printf("%d\n", qwe.a);
    printf("%c\n", qwe.c);

    char tk[32];
    if(tk[0]){
        printf("%s111", tk);
    }
}
