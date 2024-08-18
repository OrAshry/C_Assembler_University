#include "translate.h"

/**
 * @brief Frees the memory used by the machine code and data image arrays.
 *
 * This function resets the machine code and data image arrays to zero,
 * effectively freeing the resources used by these arrays. It also resets
 * the instruction counter (IC) and data counter (DC) to zero.
 *
 * @param machine_code_ptr Pointer to the translation structure that holds
 *                         the machine code and data image arrays.
 */
void free_machine_code(translation_ptr machine_code_ptr) {
    int i = 0;
    for (i = 0; i < (machine_code_ptr->IC - 100); i++) {
        machine_code_ptr->code_image[i] = 0;
    }
    for (i = 0; i < machine_code_ptr->DC; i++) {
        machine_code_ptr->data_image[i] = 0;
    }
    machine_code_ptr->IC = 0;
    machine_code_ptr->DC = 0;
}