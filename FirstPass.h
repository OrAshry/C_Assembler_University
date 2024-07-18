#include <stdio.h>
#include "SymbolSearch.c"

#define MAX_LINE_LENGTH 81
#define MAX_SYMBOL_NAME 32

/* Prototypes */
int firstPass(char * file_name, FILE * file);

struct symbol {
    char symbol_name[MAX_SYMBOL_NAME];
    int address;

    enum {
        extern_symbol,
        entry_symbol,
        code_symbol,
        data_symbol,
        entry_code,
        entry_data
    }symbol_type;
};
