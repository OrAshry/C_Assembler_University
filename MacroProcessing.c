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

struct Macro * create_macro(char * line) {
    struct Macro * macro_ptr = (struct Macro *)calloc(1, sizeof(struct Macro));
    if (macro_ptr == NULL) {
        printf("Error: Unable to allocate memory for macro\n");
        return NULL;
    }
    return macro_ptr;
}

char * get_macro_name(char * token) {
    char * name = (char *)calloc(MAX_LINE, sizeof(char));
    if (name == NULL) {
        printf("Error: Unable to allocate memory for macro name\n");
        return NULL;
    }
    strcpy(name, token);
    printf("Macro Name: %s\n", name);

    token = strtok(NULL, " ");
    if (token != NULL) {
        free(name);
        printf("Error: Macro definition isn't defined well!\n");
        return NULL;
    }
    
    if (name[strlen(name) - 1] == '\n') {
        name[strlen(name) - 1] = '\0';
    }
    
    return name;
}

int is_macro_def(char * line, struct Macro ** macro_ptr) {
    char * token = NULL;
    token = strtok(line, " ");
    char * macro_name;
    if (strncmp(token, STARTMACR, strlen(STARTMACR)) == 0) {
        token = strtok(NULL, " ");
        (*macro_ptr) = create_macro(line);
        (*macro_ptr)->lines_counter = 0;
        macro_name = get_macro_name(token);
        if(macro_name == NULL) {
            free(*macro_ptr);
            printf("Error: Unable to create macro because of additional data\n");
            printf("Moving to the next file\n");

            /******************************/
            /* need to finish and move to next file according to unviersity task.. */
            /******************************/

            return 0;
        }
        strcpy((*macro_ptr)->name, macro_name);
        return 1;
    }
    return 0;
}

int is_macro_call(char * line, struct Macro * macro_table, struct Macro * macro_ptr) {
    if (macro_ptr == NULL) {
        return 0;
    }

    get_macro_context(line, macro_ptr);

    macro_table[0] = *macro_ptr;
}

int is_macro_end(char * line) {
    char * token = NULL;
    token = strtok(line, " ");
    if (strncmp(token, ENDMACR, strlen(ENDMACR)) == 0) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            printf("Error: Macro end isn't defined well!\n");
            return 0;
        }
    }
    return 1;
}

int determine_line_type(char * line, struct Macro * macro_table, struct Macro ** macro_ptr) {
    if (is_macro_def(line, macro_ptr)) {
        return MACRO_DEF;
    } else if (is_macro_call(line, macro_table, *macro_ptr)) {
        return MACRO_CALL;
    } else if (is_macro_end(line)) {
        return MACRO_END;
    } else {
        return REGULAR_LINE;
    }
}

void fill_am_file(FILE * am_file, FILE * as_file) {
    struct Macro macro_table[MACRO_TABLE_SIZE];
    struct Macro * macro_ptr = NULL;
    char line[MAX_LINE] = {0};

    while (fgets(line, MAX_LINE, as_file) != NULL) {
        switch(determine_line_type(line, macro_table, &macro_ptr))
        {
            case MACRO_DEF:
                printf("Macro name is: %s\n", macro_ptr->name);
            case MACRO_CALL:
                break;
            case MACRO_END:
                break;
            case REGULAR_LINE:
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
    
    /* Check allocation succeded */
    if (asFileName == NULL || amFileName == NULL) {
        printf("Error: Unable to allocate memory for file names\n");
        free(asFileName);
        free(amFileName);
        return;
    }

    /* Copy read file name with ending */
    strcpy(asFileName, file_name);
    strcat(asFileName, ".as");

    /* Copy write file name with ending */
    strcpy(amFileName, file_name);
    strcat(amFileName, ".am");

    /* Read & Create file */
    as_file = read_file(asFileName);
    am_file = create_file(amFileName);

    /* Creating new am file */ 
    fill_am_file(am_file, as_file);

    /* Close files */
    fclose(as_file);
    fclose(am_file);

    /* Free memory */
    free(asFileName);
    free(amFileName); /* should we move it to main ?? */
}

int main(void) {
    macro_processing("x");
    return 0;
}