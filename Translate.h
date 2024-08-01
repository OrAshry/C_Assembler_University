#ifndef TRANSLATION_H
#define TRANSLATION_H

#define MAX_MEM_SIZE 4096

typedef struct translation {
    int code_image[MAX_MEM_SIZE];
    int data_image[MAX_MEM_SIZE];
} translation, * translation_ptr;

#endif