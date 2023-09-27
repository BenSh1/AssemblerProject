#ifndef VARIABLES_H
#define VARIABLES_H
#include <stdio.h>

#define WORD 14
#define MAX_LENGTH 200
#define WORD_SIZE 16
#define TOKEN_LEN 31

#define OK 0

enum InitOpCod
{
    Opcod0,
    Opcod1,
    Opcod10 = 10,
    Opcod11,
    Opcod12,
    Opcod13,
    Opcod14,
    Opcod15,
    Opcod16,
    Opcod17,
    Opcod18,
    Opcod19,
    Opcod20,
    Opcod21,
    Opcod22,
    Opcod23,
    Opcod24,
    Opcod30 = 30,
    Opcod31,
    Opcod32,
    Opcod63 = 63
};

enum funct
{
    fun1 = 1,
    fun2,
    fun3,
    fun4,
    fun5
};

struct opCodes {
        char *name;
        char commandGroup;
        enum funct group;
	enum InitOpCod code;
};



struct opCodes * getOpCodes();



#endif

