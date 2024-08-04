#include "SymbolTable.h"

/* Search symbols inside the symbol table*/
table_ptr symbol_search(table_ptr p, const char search_name[MAX_SYMBOL_NAME]) {
    while(p != NULL) {
        if(strcmp(search_name, p -> symbol_name) == 0) {
            return p;
        }
        p = p -> next;
    }
    return NULL;
}
