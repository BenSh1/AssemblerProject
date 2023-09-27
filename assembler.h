#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "first.h"
#include "seconde.h"
#include "labelHandler.h"
#include "commandHandler.h"
#include "dataHandler.h"

int lex_analysis(FILE *, const char * );
void freeSymbolTable();/*function from label handler*/


#endif /* ASSEMBLER_H */
