#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "FirstPass.h"

/* Prototypes */
int secondPass(char * file_name, FILE * file);
void codeWords(int num_of_words, struct ast a, int absolute_word, int relocatable_word, int external_word, int *flag, const char *name_of_file, int current_am_line);

#endif
