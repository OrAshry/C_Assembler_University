#include <stdio.h>
#include <string.h>
#include "helpingFunction.c"

#define MAX_LINE_LENGTH 81
#define MAX_SYMBOL_NAME 32

/* Prototypes */
void add_symbol_to_table(char new_name[MAX_SYMBOL_NAME], enum type new_type, int new_address, table_ptr *ptr);
int symbol_search(table_ptr p, const char search_name[MAX_SYMBOL_NAME]);

enum type{
    extern_symbol,
    entry_symbol,
    code_symbol,
    data_symbol,
    entry_code,
    entry_data
};

typedef struct symbol_table {
    char symbol_name[MAX_SYMBOL_NAME];
    enum type symbol_type;
    int symbol_address;
    struct symbol_table *next;
} symbol_table, * table_ptr;
