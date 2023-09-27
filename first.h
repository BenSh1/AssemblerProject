#ifndef FIRST_H
#define FIRST_H

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

#define INITIALIZE_IC 100
#define INITIALIZE_DC 0
#define MAX_LINE_LEN 80 
#define NUM_OF_COMMANDS 27

int firstReadLine(FILE * ,const char * );

#endif
