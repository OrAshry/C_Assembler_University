#include <stdio.h>
#include "SymbolTable.h"

int symbol_search(table_ptr p, const char *search_name) {
    int found = 0;

    while(p != NULL) {
            if(strcmp(search_name ,p -> name) == 0) {
                found = 1; 
            }
            else {
                p = p -> next;
            }
    }
    return found;
}
