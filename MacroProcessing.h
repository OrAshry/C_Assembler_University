#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"

#define DEF_MAT_SIZE 3
#define MACRO_TABLE_SIZE 2
#define SIZE_EOF 3
#define STARTMACR "macr"
#define ENDMACR "endmacr"

struct Macro
{
    char name[MAX_LINE];
    char ** context;
    int lines_counter;
};

enum MacroState
{
    MACRO_DEF,
    MACRO_CALL,
    MACRO_END,
    REGULAR_LINE,
    MACRO_BODY
};

FILE *read_file(char *file_name);
FILE *create_file(char *file_name);
void fill_am_file(FILE *am_file, FILE *as_file);
void macro_processing(char *file_name);
int determine_line_type(char *line, struct Macro **macro_table,struct Macro **macro_ptr);
int is_macro_def(char *line, struct Macro **macro_ptr);
int is_macro_body(char *line, struct Macro **macro_ptr);
int is_macro_call(char *line, struct Macro **macro_table, struct Macro **macro_ptr);
int is_macro_end(char * line, struct Macro **macro_ptr);
struct Macro *create_macro(char *token);
char *get_macro_name(char *token);
void update_macro_context(char *line, struct Macro **macro_ptr);
void append_macro_table(struct Macro **macro_table, struct Macro *macro_ptr, int macro_counter);