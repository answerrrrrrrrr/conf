#include <string.h>
#include <stdint.h>
//#include <stdbool.h>

#include "debug.h"
#include <cpu/reg.h>


//typedef unsigned short bool;
typedef uint8_t bool;
#define false 0
#define true 1


CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
