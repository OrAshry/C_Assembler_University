#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MacroProcessing.h"

FILE * read_file(char * file_name) {
    FILE * file;
    file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error: File not found\n");
        return NULL;
    }
    return file;
}

FILE * create_file(char * file_name) {
    FILE * file;
    file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Error: Unable to create file\n");
        return NULL;
    }
    return file;
}

int determine_line_type(char * line, struct Macro * macro_table, struct Macro * macro_ptr) {
    if (is_macro_def(line, macro_ptr)) {
        return MACRO_DEF;
    } else if (is_macro_call(line, macro_table, macro_ptr)) {
        return MACRO_CALL;
    } else if (is_macro_end(line)) {
        return MACRO_END;
    } else {
        return REGULAR_LINE;
    }
}

int is_macro_def(char * line, struct Macro * macro_ptr) {
    return 0;
}

int is_macro_call(char * line, struct Macro * macro_table, struct Macro * macro_ptr) {
    return 0;
}

int is_macro_end(char * line) {
    return 0;
}

void fill_am_file(FILE * am_file, FILE * as_file) {
    struct Macro macro_table[MACRO_TABLE_SIZE];
    struct Macro * macro_ptr = NULL;
    char line[MAX_LINE] = {0};

    while (fgets(line, MAX_LINE, as_file) != NULL) {
        switch(determine_line_type(line, macro_table, macro_ptr))
        {
            case MACRO_DEF:
                printf("Macro def\n");
                break;
            case MACRO_CALL:
                printf("Macro call\n");
                break;
            case MACRO_END:
                printf("Macro end\n");
                break;
            case REGULAR_LINE:
                printf("Regular line\n");
                break;
        }
    }
}

void macro_processing(char * file_name) {
    /* Files define */
    FILE * as_file; 
    FILE * am_file;

    /* File name define char */
    char * asFileName;
    char * amFileName;
    
    /* Allocate data memory tomer */
    asFileName = (char *)calloc(strlen(file_name) + SIZE_EOF, sizeof(char));
    amFileName = (char *)calloc(strlen(file_name) + SIZE_EOF, sizeof(char));
    
    if (asFileName == NULL || amFileName == NULL) {
        printf("Error: Unable to allocate memory for file names\n");
        free(asFileName);
        free(amFileName);
        return;
    }

    strcpy(asFileName, file_name);
    strcat(asFileName, ".as");

    strcpy(amFileName, file_name);
    strcat(amFileName, ".am");

    as_file = read_file(asFileName);
    am_file = create_file(amFileName);
    
    fill_am_file(am_file, as_file);

    fclose(as_file);
    fclose(am_file);
    free(asFileName);
    free(amFileName); /* should we move it to main ?? */
}

int main(void) {
    macro_processing("x");
    return 0;
}