#ifndef HELPINGFUNCTION_H
#define HELPINGFUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "LinesParser.h"

struct ast; /* Forward declaration of struct ast */

enum FunctionId
{
    CALLOC_ID,
    MALLOC_ID,
    REALLOC_ID
};

void *allocateMemory(size_t numElements, size_t sizeOfElement, int functionID);
int is_instruction(char const *str, struct ast *ast);
int is_label(char const *str, struct ast *ast, int const definition);
int is_register(char const *str);
struct string_split split_string(char * str, const char * delimiter);
void failureExit(char *message);

#endif
