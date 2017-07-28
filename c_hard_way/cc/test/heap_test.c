#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *p0 = malloc(0x118);
    char *p1 = malloc(0x120);
    free(p0);
    free(p1);
}
