#ifndef TRANSLATE_H
#define TRANSLATE_H

#define MAX_MEMORY_SIZE 4096

typedef struct translate{
    int code_image[MAX_MEMORY_SIZE];
    int data_image[MAX_MEMORY_SIZE];
    int DC;
    int IC;
} translate, * translate_ptr;

#endif TRANSLATE_H
