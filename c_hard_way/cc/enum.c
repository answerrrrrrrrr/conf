#include <stdio.h>

enum{
    a=256, b, c,
    d=512
};

int main(){
    int c = a&b;
    char not = '!';
    printf("%d\n", a&b == 256); // 0 ???
    printf("%d\n", c == 256);

    printf("%d\n", 33=='!');
    printf("%d\n", a&c);
    printf("%d", a&d>255);
}
