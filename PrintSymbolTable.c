/*for the test*/
#include "symbolTable.h"

void print_symbol_table(table_ptr p) {
    printf("Symbol Table:\n");
    while (p != NULL) {
        printf("Name: %-15s Type: %-15d Address: %-5d Next : %p\n",
               p->symbol_name, p->symbol_type, p->symbol_address, (void *)p->next);
        p = p->next;
    }
}

void print_data_image(const translation_ptr p) {
    int i;

    printf("Data Image:\n");
    for (i = 0; i < MAX_MEM_SIZE; i++) {
        if (p->data_image[i] != '\0') {
            printf("Address %d: %d\n", i, p->data_image[i]);
        }
    }
    putchar('\n');
}

void intToOctalString(int n, char *octalStr, int size) {
    int i;
    octalStr[size] = '\0';  
    for (i = size - 1; i >= 0; i--) {
        octalStr[i] = (n & 7) + '0';  /* Convert the last 3 bits to an octal digit */
        n >>= 3;  /* Shift right by 3 bits to process the next digit */
    }
}

void intToBinaryString(int n, char *binaryStr, int size) {
    int i;
    binaryStr[size] = '\0';  
    for (i = size - 1; i >= 0; i--) {
        binaryStr[i] = (n & 1) ? '1' : '0';
        n >>= 1;
    }
}

void print_code_image(const translation_ptr p) {
    int i;
    char binaryStr[16]; 

    printf("Code Image:\n");
    for (i = 0; i < MAX_MEM_SIZE; i++) {
        if (p->code_image[i] != '\0') {
            intToBinaryString(p->code_image[i], binaryStr, 15);
            printf("Address %d: %d (Binary: %s)\n", i, p->code_image[i], binaryStr);
        }
    }
    putchar('\n');
}

void fprint_code_image(const translation_ptr p, FILE *file) {
    int i;
    char octalStr[6];  /* Adjusted to 6 to accommodate 5 octal digits plus the null terminator */

    for (i = 0; i < machine_code_ptr->IC; i++) {
        if (p->code_image[i] != '\0') {
            intToOctalString(p->code_image[i], octalStr, 5);  /* Using 5 for 5-digit octal */
            fprintf(file, "%04d %s\n", i, octalStr);
        }
    }
    putchar('\n');
}

void fprint_data_image(const translation_ptr p, FILE *file) {
    int i;
    char octalStr[6];  /* 5 digits plus the null terminator */

    for (i = 0; i < machine_code_ptr->DC; i++) {
        if ((p->data_image[i] != '\0') || (p->data_image[i] == 0)) {
            intToOctalString(p->data_image[i], octalStr, 5);  /* Convert to 5-digit octal */
            fprintf(file, "%04d %s", i + machine_code_ptr->IC, octalStr);  /* Print the address with 4 digits and octal with 5 digits */
            if(i < machine_code_ptr->DC - 1) 
            {
                fprintf(file, "\n");
            }
        }
    }
    putchar('\n');
}


void print_extern_usage(extern_addresses_ptr head) {
    extern_addresses_ptr current = head;
    int i;
    printf("External Symbols Usage:\n");

    while (current != NULL) {
        printf("Name: %-15s Usage Counter: %-5d Addresses: ", 
               current->name, current->used_counter);

        for (i = 0; i < current->used_counter; i++) {
            printf("%-5d", current->used_addresses[i]);
            if (i < current->used_counter - 1) {
                printf(", ");
            }
        }

        printf("\n");
        current = current->next;
    }
    printf("\n");
}
