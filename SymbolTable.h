#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Translate.h"

#define MAX_LINE_LENGTH 82 /* 82 for 80 chars and '\n' and '\0'*/
#define MAX_BUFFER_LENGTH 83 /* 83 for 80 chars and '\n' and '\0' and if there another char*/
#define MAX_SYMBOL_NAME 32

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
table_ptr symbol_search(table_ptr p, const char search_name[MAX_SYMBOL_NAME]);
void print_symbol_table(table_ptr p); /*for the test*/
void print_data_image(const translation_ptr p);

#endif 
