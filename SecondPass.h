#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "FirstPass.h"

#define A 2
#define R 1
#define E 0

/* Prototypes */
extern_addresses_ptr secondPass(char *file_name, FILE *file, extern_addresses_ptr extern_usage_head_ptr, int *err_flag);
void codeWords(int num_of_words, struct ast a, int *flag, const char *name_of_file, int current_am_line);
void free_extern_table(extern_addresses_ptr head);

#endif
