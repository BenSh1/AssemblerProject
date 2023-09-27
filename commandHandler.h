#ifndef COMMANDSHEADER_h
#define COMMANDSHEADER_h
#include <stdio.h>
#include <stdlib.h>
#include "labelHandler.h"
#include "utility.h"

typedef struct R_OP
{
    unsigned int opcode : 6;
    unsigned int rs : 5;
    unsigned int rt : 5;
    unsigned int rd : 5;
    unsigned int funct : 5;
    unsigned int unUsed : 6;

} R_OP;

typedef struct I_OP
{
    unsigned int opcode : 6;
    unsigned int rs : 5;
    unsigned int rt : 5;
    int immed : 16;
} I_OP;

typedef struct J_OP
{
    unsigned int opcode : 6;
    unsigned int reg : 1;/* 0 = symbol , 1 = registers*/
    unsigned int address : 25;
} J_OP;


typedef struct code_line
{
    union ops
    {
        R_OP r;
        J_OP j;
        I_OP i;
    } ops;
} code_line;

typedef struct code_array
{
    code_line *arr;
    int size;
    int IC;
    unsigned int flagKnownSymbol: 2; /* 3 = it is indicate that we  will handle this code later 
					2 = find symbol and external symbol , 
					1 = find symbol and not external symbol, 
					0 = no need symbol */
} code_array;


void initializeCodeList();
void addCodeR( int IC, int opcode , int funct , int rs , int rt , int rd);
void addCodeI(int IC, int opcode ,  int rs , int rt , int immed , int flagKnownSymbol);
void addCodeJ(int IC, int opcode ,  int reg , int address , int flagKnownSymbol);
void updateCodeI(char * label , int IC);
void updateCodeJ(char * label, int IC);
int writeCommandInFile(FILE* fp);
void freeCommandList();

code_array * codeList;

#endif
