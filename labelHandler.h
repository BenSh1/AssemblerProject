#ifndef VARIABLES_H
#define VARIABLES_H
#include "first.h"
#include <string.h>
#include "error_handler.h"
#include "utility.h"




struct label
{
    char *name; /*the label itself, without colon*/
    int address;
    unsigned int data : 1; /*1 if it is data*/
    unsigned int code : 1; /*1 if it is code*/
    unsigned int external : 1; /*1 if external, 0 otherwise*/
    unsigned int entry : 1;    /*1 if entry, 0 otherwise*/
    int size;

};
struct label *labelList;
void addLabel(char *,int , unsigned int  , unsigned int ,unsigned int ,unsigned int );

char *subString(int start, int end, char *str);
int isDefinedLabel(char *purelabel);
void setEntry(char *label, int lineNum);
int setExtern(char *label, int lineNum);
int getLabelAddress(char *label);
int getLabelIndex(char * label);
void updateLabels(int ICF);
int isExternalExist();
int isEntryExist();
int isExternal(char * token);
void freeSymbolTable();
void writeEntryInFile(FILE* fp);


#endif
