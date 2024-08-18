#ifndef HELPINGFUNCTION_H
#define HELPINGFUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lineParser.h"

#define SAVE_WORDS_SIZE 20

extern char *saved_words[];

struct ast; /* Forward declaration of struct ast */

enum FunctionId
{
    CALLOC_ID,
    MALLOC_ID
};

/* Prototype Functions */
void *allocateMemory(size_t numElements, size_t sizeOfElement, int functionID);
int is_instruction(char const *str, struct ast *ast);
int is_label(char const *str, struct ast *ast, int const definition);
int is_register(char const *str);
void failureExit(char *message);
int is_saved_word(char const *str);

#endif /* HELPINGFUNCTION_H */
