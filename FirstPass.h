#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "SymbolTable.h"
#include "SymbolSearch.c"
#include "front.c"
#include "MacroProcessing.c"

/* Prototypes */
int firstPass(char * file_name, FILE * file);

#endif
