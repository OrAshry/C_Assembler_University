#include "SymbolTable.h"

/* Search symbols inside the symbol table by name */
table_ptr symbol_search(table_ptr ptr, const char search_name[MAX_SYMBOL_NAME]) {
    while(ptr) {
        if(strcmp(search_name, ptr -> symbol_name) == 0) {
            return ptr;
        }
        ptr = ptr -> next;
    }
    return NULL;
}

/* Search external sembols inside the symbol table */
table_ptr find_extern(table_ptr ptr) {
    while(ptr) {
        if(ptr -> symbol_type == extern_symbol) {
            return ptr;
        }
        ptr = ptr -> next;
    }
}

/* Search entry symbols inside the symbol table */
table_ptr find_entry(table_ptr ptr) {
    while(ptr) {
        if((ptr -> symbol_type == entry_code) || (ptr -> symbol_type == entry_data)) {
            return ptr;
        }
        ptr = ptr -> next;
    }
}
