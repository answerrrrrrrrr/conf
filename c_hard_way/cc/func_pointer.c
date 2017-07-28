#include <stdio.h>

void my_func(int );
void (*pfunc)(int );

void my_func(int a){
    printf("%d\n", a);
}

int main(){
    /**pfunc = my_func; //invalid! */
    pfunc = &my_func;
    pfunc(10);
}
