#include<stdio.h>

void set(int data, int* ptr){
    *ptr = data;
}

int main(){
    int data = 1;
    int* p = NULL;
    set(data, p);
    if(p == NULL){
        printf("y");
    }
}

