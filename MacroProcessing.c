#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MacroProcessing.h"

FILE *read_file(char *file_name)
{
    FILE *file;
    file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Error: File not found\n");
        return NULL;
    }
    return file;
}

FILE *create_file(char *file_name)
{
    FILE *file;
    file = fopen(file_name, "w");
    if (file == NULL)
    {
        printf("Error: Unable to create file\n");
        return NULL;
    }
    return file;
}

struct Macro *create_macro(char *token)
{
    char *macro_name = NULL;

    struct Macro *macro_ptr = (struct Macro *)calloc(1, sizeof(struct Macro));
    if (macro_ptr == NULL)
    {
        printf("Error: Unable to allocate memory for macro\n");
        return NULL;
    }

    macro_ptr->lines_counter = 0;
    macro_name = get_macro_name(token);
    if (macro_name == NULL)
    {
        free(macro_ptr);
        printf("Error: Unable to create macro because of additional data\n");
        printf("Moving to the next file\n");

        /******************************/
        /* need to finish and move to next file according to unviersity task.. */
        /******************************/

        return 0;
    }

    macro_ptr->context = (char **)calloc(DEF_MAT_SIZE, sizeof(char *));
    if (macro_ptr->context == NULL)
    {
        printf("Error: Unable to allocate memory for macro context\n");
        free(macro_ptr);
        return NULL; /*need to continue to next file!*/
    }

    strcpy(macro_ptr->name, macro_name);

    return macro_ptr;
}

void update_macro_context(char *line, struct Macro **macro_ptr)
{
    char **temp_ptr = NULL;

    if (macro_ptr == NULL)
    {
        return;
    }

    if ((*macro_ptr)->lines_counter >= DEF_MAT_SIZE)
    {
        temp_ptr = (char **)realloc((*macro_ptr)->context, ((*macro_ptr)->lines_counter + 1) * sizeof(char *));
        if (*temp_ptr == NULL)
        {
            printf("Error: Unable to reallocate memory for macro context\n");
            free(*macro_ptr);
            exit(0);
        }
        (*macro_ptr)->context = temp_ptr;
    }

    (*macro_ptr)->context[(*macro_ptr)->lines_counter] = (char *)calloc(MAX_LINE, sizeof(char));
    if ((*macro_ptr)->context[(*macro_ptr)->lines_counter] == NULL)
    {
        printf("Error: Unable to allocate memory for macro context\n");
        free(*macro_ptr);
        exit(0);
    }

    strcpy((*macro_ptr)->context[(*macro_ptr)->lines_counter++], line);
}

char *get_macro_name(char *token)
{
    /* If current row is empty */
    if(strcmp(token, "\n") == 0)
    {
        return NULL;
    }

    char *name = (char *)calloc(MAX_LINE, sizeof(char));
    if (name == NULL)
    {
        printf("Error: Unable to allocate memory for macro name\n");
        return NULL;
    }
    strcpy(name, token);

    token = strtok(NULL, " ");
    if (token != NULL)
    {
        free(name);
        printf("Error: Macro definition isn't defined well!\n");
        return NULL;
    }

    if (name[strlen(name) - 1] == '\n')
    {
        name[strlen(name) - 1] = '\0';
    }

    return name;
}

void append_macro_table(struct Macro **macro_table, struct Macro *macro_ptr, int macro_counter)
{
    struct Macro *temp_ptr = NULL;

    if (macro_counter >= MACRO_TABLE_SIZE)
    {
        temp_ptr = (struct Macro *)realloc(*macro_table, (macro_counter + 1) * sizeof(struct Macro *));
        if (temp_ptr == NULL)
        {
            printf("Error: Unable to reallocate memory for macro table\n");
            free(temp_ptr);
            return; /*need to continue to next file*/
        }
        *macro_table = temp_ptr;
    }

    struct Macro *temp_macro = (struct Macro *)calloc(1, sizeof(struct Macro));
    if (temp_macro == NULL)
    {
        printf("Error: Unable to allocate memory for temp macro\n");
        free(temp_macro);
        return;
        /*continue to next file*/
    }

    memcpy(temp_macro, macro_ptr, sizeof(struct Macro));
    macro_table[macro_counter] = temp_macro;
}

int is_macro_def(char *line, struct Macro **macro_ptr)
{
    if(*macro_ptr != NULL)
    {
        return 0;
    }

    char *token = NULL;
    char *temp_line = (char *)calloc(MAX_LINE, sizeof(char));

    if (temp_line == NULL)
    {
        printf("Error: Unable to allocate memory for temp line\n");
        return 0;
    }
    strcpy(temp_line, line);

    token = strtok(temp_line, " ");
    char *macro_name;
    if (strncmp(token, STARTMACR, strlen(STARTMACR)) == 0)
    {
        token = strtok(NULL, " ");
        (*macro_ptr) = create_macro(token);
        if ((*macro_ptr) == NULL)
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

int is_macro_body(char *line, struct Macro **macro_ptr)
{
    if (*macro_ptr == NULL)
    {
        return 0;
    }

    update_macro_context(line, macro_ptr);
    return 1;
}

int is_macro_call(char *line, struct Macro **macro_table, struct Macro **macro_ptr)
{
    char *token = NULL;
    int i = 0;
    char *macro_name = NULL;
    char *temp_line;

    if(*macro_ptr != NULL)
    {
        return 0;
    }

    temp_line = (char *)calloc(MAX_LINE, sizeof(char));
    if (temp_line == NULL)
    {
        printf("Error: Unable to allocate memory for temp line\n");
        return 0;
    }

    strcpy(temp_line, line);
    token = strtok(temp_line, " ");

    macro_name = get_macro_name(token);

    if (macro_name == NULL)
    {
        return 0;
    }

    /*need to find macro name in macro table*/
    for (i = 0; i < MACRO_TABLE_SIZE; i++)
    {
        if (strcmp(macro_table[i]->name, macro_name) == 0)
        {
            *macro_ptr = macro_table[i];
            return 1;
        }
    }
}

int is_macro_end(char *line, struct Macro **macro_ptr)
{
    char *token = NULL;
    char *temp_line = (char *)calloc(MAX_LINE, sizeof(char));

    if (temp_line == NULL)
    {
        printf("Error: Unable to allocate memory for temp line\n");
        return 0;
    }
    strcpy(temp_line, line);

    token = strtok(temp_line, " ");

    if (token[strlen(token) - 1] == '\n')
    {
        token[strlen(token) - 1] = '\0';
    }

    if (strncmp(token, ENDMACR, strlen(ENDMACR)) == 0)
    {
        token = strtok(NULL, " ");
        if (token != NULL)
        {
            printf("Error: Macro end isn't defined well!\n");
            return 0;
        }

        return 1;
    }

    return 0;
}

int determine_line_type(char *line, struct Macro **macro_table, struct Macro **macro_ptr)
{
    if (is_macro_def(line, macro_ptr))
    {
        return MACRO_DEF;
    }
    else if (is_macro_end(line, macro_ptr))
    {
        return MACRO_END;
    }
    else if (is_macro_call(line, macro_table, macro_ptr))
    {
        return MACRO_CALL;
    }
    else if (is_macro_body(line, macro_ptr))
    {
        return MACRO_BODY;
    }
    else
    {
        return REGULAR_LINE;
    }
}

void fill_am_file(FILE *am_file, FILE *as_file)
{
    struct Macro **macro_table;
    struct Macro *macro_ptr = NULL;
    char line[MAX_LINE] = {0};
    int macro_counter = 0;

    macro_table = (struct Macro **)calloc(MACRO_TABLE_SIZE, sizeof(struct Macro *));
    if (macro_table == NULL)
    {
        printf("Error: Unable to allocate memory for macro table\n");
        /*continue to next file*/
        return;
    }

    while (fgets(line, MAX_LINE, as_file) != NULL)
    {
        switch (determine_line_type(line, macro_table, &macro_ptr))
        {
        case MACRO_DEF:
            break;
        case MACRO_CALL:
            printf("MACRO_CALL\n");
            printf("Macro name: %s\n", macro_ptr->name);
            break;
        case MACRO_END:
            append_macro_table(macro_table, macro_ptr, macro_counter);
            macro_counter++;
            macro_ptr = NULL;
            break;
        case REGULAR_LINE:
            fputs(line, am_file);
            break;
        case MACRO_BODY:
            break;
        }
        memset(line, 0, sizeof(line)); /* initalize line variable */
    }
}

void macro_processing(char *file_name)
{
    /* Files define */
    FILE *as_file;
    FILE *am_file;

    /* File name define char */
    char *asFileName;
    char *amFileName;

    /* Allocate data memory */

    asFileName = (char *)calloc(strlen(file_name) + SIZE_EOF, sizeof(char));
    amFileName = (char *)calloc(strlen(file_name) + SIZE_EOF, sizeof(char));

    /* Check allocation succeded */
    if (asFileName == NULL || amFileName == NULL)
    {
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

int main(void)
{
    macro_processing("x");
    return 0;
}