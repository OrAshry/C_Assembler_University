#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "Translate.h"
#include "SymbolTable.h"
#include "LinesParser.h"

/* Prototypes */
int firstPass(char * file_name, FILE * file);


extern table_ptr head_ptr;
extern table_ptr found;

#endif
