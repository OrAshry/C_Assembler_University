#include <stdio.h>
#include "SymbolTable.h"

/* Search symbols inside the symbol table*/
table_ptr symbol_search(table_ptr p, const char search_name[MAX_SYMBOL_NAME]) {
    table_ptr found = NULL;

    while(p != NULL) {
            if(strcmp(search_name ,p -> symbol_name) == 0) {
                found = p; 
            }
            else {
                p = p -> next;
            }
    }
    return found;
}
