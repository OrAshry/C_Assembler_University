#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80
#define DEF_MAT_SIZE 3
#define MACRO_TABLE_SIZE 2
#define SIZE_EOF 3

FILE * read_file(char * file_name);
FILE * create_file(char * file_name);
void fill_am_file(FILE * am_file, FILE * as_file);
void macro_processing(char * file_name);

struct Macro {
    char name[MAX_LINE];
    char context[DEF_MAT_SIZE][DEF_MAT_SIZE];
    int lines_counter;
};

enum MacroState {
    MACRO_DEF,
    MACRO_CALL,
    MACRO_END,
    REGULAR_LINE
};

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

void fill_am_file(FILE * am_file, FILE * as_file) {
    struct Macro macro_table[MACRO_TABLE_SIZE];
    struct Macro * macro_ptr = NULL;
    char line[MAX_LINE] = {0};

    while (fgets(line, MAX_LINE, as_file) != NULL) {
        fputs(line, am_file);
    }
}

void macro_processing(char * file_name) {
    FILE * as_file; /**/
    FILE * am_file;
    char * asFileName;
    char * amFileName;
    
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

    close(as_file);
    close(am_file);
    free(asFileName);
    free(amFileName); /* should we move it to main ?? */
}

int main(void) {
    macro_processing("x");
    return 0;
}