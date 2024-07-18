#include <stdio.h>
#include "SymbolTable.h"

int symbol_search(table_ptr p, char name[MAX_SYMBOL_NAME], int num) {
    int found = 0;
    int i;

    for(i;i < num;++i) {
        if(strcmp(name,p) == 0) {
            found = 1;
            continue;
        }
    }
    return found;
}
