#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#define MAX_MEMORY_SIZE 4096

#include <stdio.h>
#include "SymbolTable.h"
#include "front.h"

typedef struct translate{
    int code_image[MAX_MEMORY_SIZE];
    int data_image[MAX_MEMORY_SIZE];
    int DC;
    int IC;
} translate, * translate_ptr;

/* Prototypes */
int firstPass(char * file_name, FILE * file, translate_ptr ptr_to_translate);

#endif
