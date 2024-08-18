#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "Translate.h"
#include "SymbolTable.h"
#include "lineParser.h"
#include "output.h"
#include "macroProcessing.h"

/* Prototypes */
int firstPass(char * file_name, FILE * file, struct MacroContext *macro_table);

extern table_ptr head_ptr;
extern table_ptr found;

#endif
