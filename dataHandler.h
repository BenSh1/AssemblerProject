#ifndef COMMANDSHEADER_h
#define COMMANDSHEADER_h
#include <stdio.h>
#include <stdlib.h>

typedef struct data_line
{
    int *data;
    int size;
    int typeByte;/* 1 = db , 2 = dh , 4 = dw*/
    int DC;
    int address;
} data_line;

typedef struct data_array
{
    data_line *arr;
    int curr_index;
    int size;
    int DC;
} data_array;


enum {DB = 1, DH = 2, DW = 4}; /*data image bytes*/

void initializeDataList();
void addData(int DC, int numOfParm , int typeByte ,int * parms);
void updateData(int ICF);
void writeDataInFile(FILE* fp);
void freeDataList();

#endif
