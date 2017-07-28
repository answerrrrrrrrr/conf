#include "stdio.h"

int main()
{
    /* begin */
    const int i1 = 40;
    int *pi;
    pi = &i1; /* 这样可以吗？不行，VC下是编译错。*/
    /* const int 类型的i1的地址是不能赋值给指向int 类型地址的指针pi的。否则pi岂不是能修改i1的值了吗！*/
    pi = (int *) &i1; /* 这样可以吗？强制类型转换可是C所支持的。*/
    /* VC下编译通过，但是仍不能通过 *pi = 80来修改i1的值。去试试吧！看看具体的怎样。*/
    /* end */
}

