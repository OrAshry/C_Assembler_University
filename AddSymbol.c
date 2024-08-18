#include "SymbolTable.h"

void add_symbol_to_table(char new_name[MAX_SYMBOL_NAME], int new_type, int new_address, table_ptr *ptr) {
    
    table_ptr new_symbol = (table_ptr)allocateMemory(1, sizeof(symbol_table), MALLOC_ID);
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

void add_symbol_to_extern_usage(char new_name[MAX_SYMBOL_NAME], int new_address, extern_addresses_ptr * ptr) {
    extern_addresses_ptr new_extern = (extern_addresses_ptr)allocateMemory(1, sizeof(extern_addresses), MALLOC_ID);
    strcpy(new_extern->name, new_name);
    new_extern->used_addresses[0] = new_address;
    new_extern->used_counter = 1;
    new_extern->next = NULL;

    /* The table is empty */
    /*if(*ptr == NULL) {
        *ptr = new_extern;
    }*/
    if(strcmp((*ptr)->name, "") == 0) {
        *ptr = new_extern;
    }

    /* The table is not empty */
    else {
        extern_addresses_ptr current = *ptr;
        while(current -> next != NULL) 
        {
            current = current -> next;
        }
        current -> next = new_extern;
    }
}

void free_symbol_table(table_ptr *head) {
    table_ptr current = *head;
    table_ptr next;

    while (current != NULL) {
        next = current->next;
        free(current);       
        current = next;       
    }

    *head = NULL;
}
