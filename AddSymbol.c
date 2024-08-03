#include "SymbolTable.h"

void add_symbol_to_table(char new_name[MAX_SYMBOL_NAME], int new_type, int new_address, table_ptr *ptr) {
    
    table_ptr new_symbol = (table_ptr)malloc(sizeof(symbol_table));
    strcpy(new_symbol -> symbol_name, new_name);
    new_symbol -> symbol_type = (enum type)new_type;
    new_symbol -> symbol_address = new_address;
    new_symbol-> next = NULL;
    
    /* The table is empty */
    if(*ptr == NULL) {
        *ptr = new_symbol;
    }
    
    /* The table is not empty */
    else {
        table_ptr current = *ptr;
        while(current -> next != NULL) {
             current = current -> next;
        }
        current -> next = new_symbol;
    }
}
