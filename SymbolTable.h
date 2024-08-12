#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <string.h>
#include "helpingFunction.h"
#include "Translate.h"

#define MAX_LINE_LENGTH 82 /* 82 for 80 chars and '\n' and '\0'*/
#define MAX_BUFFER_LENGTH 83 /* 83 for 80 chars and '\n' and '\0' and if there another char*/
#define MAX_SYMBOL_NAME 31

enum type{
    extern_symbol,
    entry_symbol,
    code_symbol,
    data_symbol,
    entry_code,
    entry_data
};

typedef struct symbol_table {
    char symbol_name[MAX_SYMBOL_NAME];
    enum type symbol_type;
    int symbol_address;
    struct symbol_table *next;
} symbol_table, * table_ptr;

/* Prototypes */
void add_symbol_to_table(char new_name[MAX_SYMBOL_NAME], int new_type, int new_address, table_ptr *ptr);
table_ptr symbol_search(table_ptr ptr, const char search_name[MAX_SYMBOL_NAME]);
table_ptr find_extern(table_ptr ptr);
table_ptr find_entry(table_ptr ptr);
int symbol_counter(table_ptr ptr);
void print_symbol_table(table_ptr p); /*for the test*/
void print_data_image(const translation_ptr p);
void print_code_image(const translation_ptr p);

#endif 
