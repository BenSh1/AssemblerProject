#ifndef UTILITY_H
#define UTILITY_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "error_handler.h"

char * subString(int start, int end, char *str );
int validNumber(char * token , int currIndex);
void remove_spaces(char* s);

#endif
