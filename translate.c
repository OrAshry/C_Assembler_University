#include "translate.h"

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