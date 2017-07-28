#include <stdio.h>

typedef union{
    unsigned int i;
    unsigned long l;
    struct {
        char a;
        char b;
    };
} Reg;


int main()
{
    Reg u;
    u.l = 10;
    u.i = 1;

    printf("%x\n", u);
    printf("%x\n", &u);
    printf("%x\n", u.i);
    printf("%x\n", (&u)->i);
    printf("%lx\n", u.l);
    /*printf("%d\n", sizeof(Reg));*/

    printf("%x\n", u.b);
    
}
