#include <stdio.h>
#include <ctype.h>
#include <string.h>

int can_print_it(char ch)
{
    return isalpha(ch) || isblank(ch);
}

void print_letters(char arg[], int len)
{
    printf("Len: %d\t", len);

    int i = 0;

    for(i = 0; i < len; i++){
    /*for(i = 0; arg[i] != '\0'; i++){*/
        char ch = arg[i];

        if(can_print_it(ch)) {
            printf("'%c' == %d ", ch, ch);
        }
    }

    printf("\n");
}

void print_arguments(int argc, char *argv[])
{
    int i = 0;

    for(i = 0; i < argc; i++){
        print_letters(argv[i], strlen(argv[i]));
    }
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}
