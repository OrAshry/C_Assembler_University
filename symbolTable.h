#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <string.h>
#include "helpingFunction.h"
#include "Translate.h"

#define MAX_LINE_LENGTH 82 /* 82 for 80 chars and '\n' and '\0'*/
#define MAX_BUFFER_LENGTH 83 /* 83 for 80 chars and '\n' and '\0' and if there another char*/
#define MAX_SYMBOL_NAME 31

/* Types of symbols */
enum type{
    extern_symbol,
    entry_symbol,
    code_symbol,
    data_symbol,
    entry_code,
    entry_data
};

/**
 * @brief Structure representing a symbol table.
 *
 * This structure holds the name, type, and address of a symbol in the assembly program,
 * along with a pointer to the next symbol in the symbol table linked list.
 */
typedef struct symbol_table {
    char symbol_name[MAX_SYMBOL_NAME];
    enum type symbol_type;
    int symbol_address;
    struct symbol_table * next;
} symbol_table, * table_ptr;

/**
 * @brief Structure representing externals and the address they have been used.
 *
 * This structure holds the name of the external symbol, an array of addresses where it is used,
 * a counter for the number of uses, and a pointer to the next external address entry in the linked list.
 */
typedef struct extern_adresses {
    char name[MAX_SYMBOL_NAME];
    int used_addresses[MAX_BUFFER_LENGTH];
    int used_counter;
    struct extern_adresses * next;
} extern_addresses, * extern_addresses_ptr;

/* Prototypes */
void add_symbol_to_table(char new_name[MAX_SYMBOL_NAME], int new_type, int new_address, table_ptr *ptr);
void add_symbol_to_extern_usage(char new_name[MAX_SYMBOL_NAME], int new_address, extern_addresses_ptr * ptr);
table_ptr symbol_search(table_ptr ptr, const char search_name[MAX_SYMBOL_NAME]);
table_ptr find_extern_in_symbol_table(table_ptr ptr);
extern_addresses_ptr find_extern(extern_addresses_ptr ptr, const char search_name[MAX_SYMBOL_NAME]);
table_ptr find_entry(table_ptr ptr);
void print_symbol_table(table_ptr p); /*for the test*/
void print_data_image(const translation_ptr p);
void print_code_image(const translation_ptr p);
void print_extern_usage(extern_addresses_ptr head);
void fprint_code_image(const translation_ptr p, FILE *file);
void fprint_data_image(const translation_ptr p, FILE *file);
void free_symbol_table(table_ptr *head);

#endif 
