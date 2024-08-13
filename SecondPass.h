#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "FirstPass.h"

#define A 2
#define R 1
#define E 0

#define ADDRESS_BIT_LOCATION 3


/* Prototypes */
int secondPass(char * file_name, FILE * file);
void codeWords(int num_of_words, struct ast a, int *flag, const char *name_of_file, int current_am_line);

#endif
