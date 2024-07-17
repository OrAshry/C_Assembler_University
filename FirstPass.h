#include <stdio.h>

#define MAX_LINE_LENGTH 81
#define MAX_SYMBOL_NAME 32

/* Prototypes */
int firstPass(char * file_name, FILE * file);

struct symbol {
    char symbol_name[SYMBOL_NAME_MAX];
    int address;

    enum {
        extern;
        entry;
        .code;
        .data;
        entry_code;
        entry_data;
    }symbol_type;
};
