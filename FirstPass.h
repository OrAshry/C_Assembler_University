include <stdio.h>

define MAX_LINE_LENGTH 81
define MAX_SYMBOL_NAME 32

/* Prototypes */
int firstPass(char * file_name, FILE * file);

struct symbol {
    char symbol_name[SYMBOL_NAME_MAX];
    int address;

    enum {
        extern_symbol;
        entry_symbol;
        code_symbol;
        data_symbol;
        entry_code_symbol;
        entry_data_symbol;
    }symbol_type;
};
