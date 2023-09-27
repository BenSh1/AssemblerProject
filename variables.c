#include "variables.h"


struct opCodes opCodesArray[] = {
    {"add", 'R', fun1, Opcod0},
    {"sub", 'R', fun2, Opcod0},
    {"and", 'R', fun3, Opcod0},
    {"or", 'R', fun4, Opcod0},
    {"nor", 'R', fun5, Opcod0},
    {"move", 'R', fun1, Opcod1},
    {"mvhi", 'R', fun2, Opcod1},
    {"mvlo", 'R', fun3, Opcod1},
    {"addi", 'I', 0, Opcod10},
    {"subi", 'I', 0, Opcod11},
    {"andi", 'I', 0, Opcod12},
    {"ori", 'I', 0, Opcod13},
    {"nori", 'I', 0, Opcod14},
    {"bne", 'I', 0, Opcod15},
    {"beq", 'I', 0, Opcod16},
    {"blt", 'I', 0, Opcod17},
    {"bgt", 'I', 0, Opcod18},
    {"lb", 'I', 0, Opcod19},
    {"sb", 'I', 0, Opcod20},
    {"lw", 'I', 0, Opcod21},
    {"sw", 'I', 0, Opcod22},
    {"lh", 'I', 0, Opcod23},
    {"sh", 'I', 0, Opcod24},
    {"jmp", 'J', 0, Opcod30},
    {"la", 'J', 0, Opcod31},
    {"call", 'J', 0, Opcod32},
    {"stop", 'J', 0, Opcod63},
};
 
struct opCodes * getOpCodes()
{
	return opCodesArray;
}



