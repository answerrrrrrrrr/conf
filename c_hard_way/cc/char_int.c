#include <stdio.h>
#include <stdlib.h>

int main()
{
    char a = '1';
    char *b = "0x1000";  // failed
    printf("%c\n", a);
    printf("%d\n", atoi(b));

    unsigned int i = -1;
    printf("%D\n", i);

    char *args = "1 i0x1";
    unsigned int n, addr;
    sscanf(args, "%u %x", &n, &addr);
    printf("%u %u", n, addr);

    char *c = "0x100qwe";
    char *end;
    printf("\n%d", strtol(c, &end, 0));
}
