#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "syntax_analysis.h"
#include "error_handler.h"
#include "variables.h"
#define IS_INSTRUCTION(str) (( (strcmp(str, "add")) && (strcmp(str, "sub")) && (strcmp(str, "and"))    \
                        && (strcmp(str, "or")) && (strcmp(str, "nor")) && (strcmp(str, "move"))    \
                        && (strcmp(str, "mvhi")) && (strcmp(str, "mvlo")) && (strcmp(str, "addi")) \
                        && (strcmp(str, "subi")) && (strcmp(str, "andi")) && (strcmp(str, "ori"))  \
                        && (strcmp(str, "nori")) && (strcmp(str, "bne")) && (strcmp(str, "beq"))   \
                        && (strcmp(str, "blt")) && (strcmp(str, "bgt")) && (strcmp(str, "lb"))     \
                        && (strcmp(str, "sb")) && (strcmp(str, "lw")) && (strcmp(str, "sw"))       \
                        && (strcmp(str, "lh")) && (strcmp(str, "sh")) && (strcmp(str, "jmp"))      \
                        && (strcmp(str, "la")) && (strcmp(str, "call")) && (strcmp(str, "stop")) ))

#define IS_DIRECTIVE(str) (( (strcmp(str, ".dh")) && (strcmp(str, ".dw")) && (strcmp(str, ".db")) \
                          && (strcmp(str, ".asciz")) && (strcmp(str, ".entry")) && (strcmp(str, ".extern")) ))


int lex_analysis(FILE *, const char * );
int is_not_comment_or_empty_line(const char * );
int is_label( const char * , int );

#endif 
