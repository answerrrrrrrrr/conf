#include "stdio.h"
unsigned short *sum(unsigned char a, unsigned char b)
{
    unsigned short s = 0;
    s = a + b;
    return &s;
}
int main()
{
    unsigned short *p=NULL;
    unsigned char a=1, b=2;
    p = sum(a, b);
    printf("%u+%u", a, b);
    printf("=%u\n", *p);
    return 0;
}
