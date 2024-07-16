include <stdio.h>
define MAX_SYMBOL_NAME 32

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
