#include<stdio.h>


int main(){
    char name[6] = "text";

    printf("name addr:0x%X\n", name);
    printf("name addr:0x%X\n", &name[0]);
    printf("name addr:0x%X\n", &name);
    printf("name addr:0x%X\n", name+1);
    printf("name addr:0x%X\n", &name+1);
    printf("size of %d", sizeof(name));

}

