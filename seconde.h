
#ifndef SECONDE_H
#define SECONDE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "syntax_analysis.h"
#include "lexical_analysis.h"
#include "variables.h"
#include "utility.h"
#include "labelHandler.h"
#include "commandHandler.h"
#include "dataHandler.h"

#define INITIALIZE_IC 100
#define INITIALIZE_DC 0
#define MAX_LINE_LEN 80 

void secondeReadLine(FILE *fp ,const char * file_n);
void initializeExternalList();
void addExtern(char* label , int address);
void writeEntryInFile( FILE *);
int isEntryExist();
void writeDataInFile( FILE * );
int isExternal(char *);
int isDefinedLabel(char *);
void setEntry(char *token,int line_num);
int isExternalExist();
void handle_directive_sentence_seconde(char*  , int* );
void handle_IType_instruction_seconde(char * , int , int  , int *);
void handle_JType_instruction_seconde(char * , int  , int * , int);
void creates_files(const char *);



#endif 
