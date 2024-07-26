#include <stdio.h>
#include <string.h>
#include "helpingFunction.c"

#define MAX_LINE_LENGTH 81
#define MAX_SYMBOL_NAME 32

/* Prototypes */
void add_symbol_to_table(symbol new);
int symbol_search(table_ptr p, const char search_name[MAX_SYMBOL_NAME]);

enum type{
    extern_symbol,
    entry_symbol,
    code_symbol,
    data_symbol,
    entry_code,
    entry_data
};

typedef struct symbol {
    char symbol_name[MAX_SYMBOL_NAME];
    enum type symbol_type;
} symbol, * symbol_ptr;

typedef struct symbol_table {
    symbol;
    int address;
    struct symbol_table *next;
} symbol_table, * table_ptr;
