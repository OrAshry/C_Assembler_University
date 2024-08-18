#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "translate.h"
#include "symbolTable.h"
#include "lineParser.h"
#include "output.h"
#include "macroProcessing.h"

/* Prototypes */
int firstPass(char * file_name, FILE * file, struct MacroContext *macro_table);

/* External variable declarations */
extern table_ptr head_ptr; /* Pointer to the head of the symbol table */
extern table_ptr found; /* Pointer to the found symbol in the table */

#endif
