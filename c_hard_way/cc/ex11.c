#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;
    while(i < argc){
        printf("argc %d: %s\n", i, argv[i]);
        i++;
    }

    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas"
    };

    int num_states = 4;
    i = 0;
    while(i < num_states){
        states[i] = argv[i];
        printf("state %d: %x\n", i, states[i]);
        i++;
    }

    return 0;
}
