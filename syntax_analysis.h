#ifndef SYNTAX_ANALYSE_H
#define SYNTAX_ANALYSE_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "error_handler.h"
#include "variables.h"
#include "lexical_analysis.h"
#include "utility.h"

#define BYTE_LENGTH 8
#define BINARY_NUM 2 
#define MAX_NUM(bytes) ((1 << (BYTE_LENGTH * bytes - 1)) - 1) /*get max possible num of signed int by amount of bytes */
#define STRING_START (strlen(".asciz "))


int analyse_type(char * token, const int cmd_type, char this_line[]);
int syntax_analyse_dir(char * token, char this_line[]);
int syntax_analyse_ins(char * token , char[]);
int check_start(char *token);
int string_analysis(char this_line[]);
int numbers_analysis(char * token, int bytes );


enum {LABEL_CMD = 1, DIRECTIVE_CMD = 2, INSTRUCTION_CMD = 3}; /*line types */
enum {DB = 1, DH = 2, DW = 4}; /*data image bytes*/

#endif /* SYNTAX_ANALYSE_H */
