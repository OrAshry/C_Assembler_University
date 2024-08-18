#ifndef TRANSLATION_H
#define TRANSLATION_H

#define MAX_MEM_SIZE 4096

/**
 * @brief A structure to hold the translation of machine code and data.
 *
 * This structure contains arrays for storing the machine code and data image,
 * as well as Instruction counter and Data counter.
 */
typedef struct translation {
    int code_image[MAX_MEM_SIZE];
    int IC;
    int data_image[MAX_MEM_SIZE];
    int DC;
} translation, * translation_ptr;

/* External references to the machine code and its pointer */
extern translation machine_code;
extern translation_ptr machine_code_ptr;

void free_machine_code(translation_ptr machine_code_ptr);

#endif
