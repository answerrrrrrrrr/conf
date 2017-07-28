#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *p = malloc(sizeof(void *));
    *p = 1;
    printf("%d\n", *p);
    printf("%p\n", p);
    free(p);
    printf("%p\n", p);
    p = NULL;
    printf("%p\n", p);
    printf("%p\n", &p);

    int arr[100] = {1,2,3};
    printf("%d\n", arr[1]);
    printf("%d\n", arr);
    printf("%d\n", &arr);

    int **q;
    
    q = &arr;
    printf("%d\n", q[1]);
}
