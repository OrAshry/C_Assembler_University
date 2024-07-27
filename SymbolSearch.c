#include <stdio.h>
#include "SymbolTable.h"

/* Search symbols inside the symbol table*/
int symbol_search(table_ptr p, const char search_name[MAX_SYMBOL_NAME]) {
    int found = 0;

    while(p != NULL) {
            if(strcmp(search_name ,p -> symbol_name) == 0) {
                found = 1; 
            }
            else {
                p = p -> next;
            }
    }
    return found;
}
