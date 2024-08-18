#include "symbolTable.h"

/**
 * @brief Searches for a symbol in the symbol table by its name.
 *
 * This function traverses the symbol table linked list, comparing each symbol's name with the 
 * provided search name. If a match is found, a pointer to the corresponding symbol table node 
 * is returned. If no match is found, the function returns NULL.
 *
 * @param ptr A pointer to the head of the symbol table linked list.
 * @param search_name The name of the symbol to search for.
 * @return A pointer to the symbol table node if found, otherwise NULL.
 */
table_ptr symbol_search(table_ptr ptr, const char search_name[MAX_SYMBOL_NAME]) {
    while(ptr) {
        if(strcmp(search_name, ptr -> symbol_name) == 0) {
            return ptr;
        }
        ptr = ptr -> next;
    }
    return NULL;
}

/**
 * @brief Searches if there is any entry symbol in the symbol table.
 *
 * This function traverses the symbol table linked list, searching for the first symbol that is 
 * marked as either `entry_code` or `entry_data`. If such a symbol is found, a pointer to that 
 * symbol table node is returned. If no entry symbol is found, the function returns NULL.
 *
 * @param ptr A pointer to the head of the symbol table linked list.
 * @return A pointer to the entry symbol table node if found, otherwise NULL.
 */
table_ptr find_entry(table_ptr ptr) {
    while(ptr) {
        if((ptr -> symbol_type == entry_code) || (ptr -> symbol_type == entry_data)) {
            return ptr;
        }
        ptr = ptr -> next;
    }
    return NULL;
}

/**
 * @brief Searches if there is any extern symbol in the symbol table.
 *
 * This function traverses the symbol table linked list, searching for the first symbol that is 
 * marked as an `extern_symbol`. If such a symbol is found, a pointer to that symbol table node 
 * is returned. If no external symbol is found, the function returns NULL.
 *
 * @param ptr A pointer to the head of the symbol table linked list.
 * @return A pointer to the external symbol table node if found, otherwise NULL.
 */
table_ptr find_extern_in_symbol_table(table_ptr ptr) {
    while(ptr) 
    {
        if(ptr -> symbol_type == extern_symbol)
        {
            return ptr;
        }
        ptr = ptr -> next;
    }
    return NULL;
}

/**
 * @brief Searches for an external symbol in the external addresses list by its name.
 *
 * This function traverses the linked list of external addresses, comparing each symbol's name 
 * with the provided search name. If a match is found, a pointer to the corresponding external 
 * addresses node is returned. If no match is found, the function returns NULL.
 *
 * @param ptr A pointer to the head of the external addresses linked list.
 * @param search_name The name of the external symbol to search for.
 * @return A pointer to the external addresses node if found, otherwise NULL.
 */
extern_addresses_ptr find_extern(extern_addresses_ptr ptr, const char search_name[MAX_SYMBOL_NAME]) {
    while(ptr) {
        if(strcmp(ptr->name, search_name) == 0) {
            return ptr;
        }
        ptr = ptr -> next;
    }
    return NULL;
}
