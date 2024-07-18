#include <stdio.h>

#define MAX_LINE_LENGTH 81
#define MAX_SYMBOL_NAME 32

enum type{
    extern_symbol,
    entry_symbol,
    code_symbol,
    data_symbol,
    entry_code,
    entry_data
};

struct symbol {
    char symbol_name[MAX_SYMBOL_NAME];
    int address;
    enum type symbol_type;
};

typedef struct symbol_table {
    char name[MAX_SYMBOL_NAME];
    int address;
    enum type the_type;
    struct symbol_table *next;
} symbol_table;
