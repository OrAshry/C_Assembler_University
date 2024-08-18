#ifndef MACROPROCESSING_H
#define MACROPROCESSING_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Constants.h"
#include "helpingFunction.h"
#include "macroContext.h"

#define DEF_MAT_SIZE 15
#define MACRO_TABLE_SIZE 200
#define SIZE_EOF 3
#define STARTMACR "macr"
#define ENDMACR "endmacr"

/* Macro Structure */
struct Macro
{
    char name[MAX_LINE];
    char **context;
    int lines_counter;
};

/* Macro State Enumeration */
enum MacroState
{
    MACRO_DEF,
    MACRO_CALL,
    MACRO_END,
    REGULAR_LINE,
    MACRO_BODY
};

/* Functions Prototype */
FILE *open_file(char *file_name, char *mode);
struct MacroContext fill_am_file(FILE *am_file, FILE *as_file, int *result, int *macro_counter);
char * macro_processing(char *file_name, struct MacroContext *macro_table);
int determine_line_type(char *line, struct Macro **macro_table, struct Macro **macro_ptr, const int macro_counter);
int is_macro_def(char *line, struct Macro **macro_ptr, struct Macro **macro_table, const int macro_counter);
int is_macro_body(char *line, struct Macro **macro_ptr);
int is_macro_call(char *line, struct Macro **macro_table, struct Macro **macro_ptr, const int macro_counter);
int is_macro_end(char *line, struct Macro **macro_ptr);
struct Macro *create_macro(char *token, int *result, struct Macro **macro_table, const int macro_counter);
char *get_macro_name(char *token);
void update_macro_context(char *line, struct Macro **macro_ptr);
void free_macro_table(struct MacroContext *macro_table);
void append_macro_table(struct Macro **macro_table, struct Macro *macro_ptr, int macro_counter);
int check_duplicate_macro(const char *macro_name, struct Macro **macro_table, const int macro_counter);

#endif
