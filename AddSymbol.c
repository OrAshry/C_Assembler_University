#include "SymbolTable.h"

/**
 * @brief a new symbol to the symbol table.
 *
 * This function creates a new symbol with the specified name, type, and address,
 * and appends it to the end of the symbol table.
 *
 * @param new_name     The name of the new symbol. It must be a string of length
 *                     MAX_SYMBOL_NAME.
 * @param new_type     The type of the new symbol, which is an integer corresponding
 *                     to an enum type.
 * @param new_address  The address associated with the new symbol.
 * @param ptr          A pointer to the head of the symbol table. The function will
 *                     update this pointer if the table was empty.
 */
void add_symbol_to_table(char new_name[MAX_SYMBOL_NAME], int new_type, int new_address, table_ptr *ptr)
{
    table_ptr new_symbol;
    table_ptr current;

    /* Allocate memory for the new symbol */
    new_symbol = (table_ptr)allocateMemory(1, sizeof(symbol_table), MALLOC_ID);

    strcpy(new_symbol->symbol_name, new_name);
    new_symbol->symbol_type = (enum type)new_type;
    new_symbol->symbol_address = new_address;
    new_symbol->next = NULL;

    /* The table is empty */
    if (*ptr == NULL)
    {
        *ptr = new_symbol;
    }

    else
    {
        current = *ptr;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_symbol;
    }
}

/**
 * @brief a new extern symbol to the extern usage table.
 *
 * This function creates a new extern entry with the specified name and address,
 * and appends it to the end of the extern usage table.
 *
 * @param new_name     The name of the extern symbol. It must be a string of length
 *                     MAX_SYMBOL_NAME.
 * @param new_address  The address where the extern symbol is used.
 * @param ptr          A pointer to the head of the extern addresses table. The function
 *                     will update this pointer if the table was empty.
 */
void add_symbol_to_extern_usage(char new_name[MAX_SYMBOL_NAME], int new_address, extern_addresses_ptr *ptr)
{

    /* Allocate memory for the new extern entry */
    extern_addresses_ptr new_extern = (extern_addresses_ptr)allocateMemory(1, sizeof(extern_addresses), MALLOC_ID);

    /* Set the extern entry's attributes */
    strcpy(new_extern->name, new_name);
    new_extern->used_addresses[0] = new_address;
    new_extern->used_counter = 1;
    new_extern->next = NULL;

    /* The table is empty */
    /*if(*ptr == NULL) {
        *ptr = new_extern;
    }*/
    if (strcmp((*ptr)->name, "") == 0)
    {
        *ptr = new_extern;
    }

    /* The table is not empty */
    else
    {
        extern_addresses_ptr current = *ptr;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_extern;
    }
}

/**
 * @brief all memory allocated for the symbol table.
 *
 * This function iterates through the symbol table, freeing each node and setting
 * the head pointer to NULL once the table is empty.
 *
 * @param head  A pointer to the head of the symbol table. The function will update
 *              this pointer to NULL after freeing the table.
 */
void free_symbol_table(table_ptr *head)
{
    table_ptr current;
    table_ptr next;

    /* Initialize current to the head of the table */
    current = *head;

    /* Traverse and free each node in the symbol table */
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}
