#include "stringSplit.h"
#include <stdio.h>

#define DEF_MAT_SIZE 3
#define MACRO_TABLE_SIZE 100
#define SIZE_EOF 3
#define STARTMACR "macr"
#define ENDMACR "endmacr"
#define SPACES " \t\v\f"

char not_valid_words[50][50] = {"data", "string", "entry", "macr", "endmacr", "extern",
                                "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
                                "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

struct Macro
{
    char name[MAX_LINE];
    char **context;
    int lines_counter;
};

enum MacroState
{
    MACRO_DEF,
    MACRO_CALL,
    MACRO_END,
    REGULAR_LINE,
    MACRO_BODY
};

FILE *read_file(char *file_name);
FILE *create_file(char *file_name);
void macro_processing(char *file_name);
void fill_am_file(FILE *am_file, FILE *as_file);

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

int check_macro_name(char *name)
{
    int i;
    for (i = 0; i < 50; i++)
    {
        if (strcmp(name, not_valid_words[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}

struct Macro *create_macro(struct string_split split_string)
{
    char macro_name[MAX_LINE] = {0};
    struct Macro *macro_ptr = (struct Macro *)allocateMemory(1, sizeof(struct Macro), CALLOC_ID);
    macro_ptr->lines_counter = 0;

    if (split_string.size > 2)
    {
        printf("Error: Unable to create macro because of additional data\n");
        printf("Moving to the next file\n");
        return NULL;
    }

    if (check_macro_name(split_string.string[1]) == 0)
    {
        printf("Error: Macro name is not valid word - no INS / DIR \n");
        return NULL;
    }

    macro_ptr->context = (char **)allocateMemory(DEF_MAT_SIZE, sizeof(char *), CALLOC_ID);
    strcpy(macro_ptr->name, macro_name);

    return macro_ptr;
}

int is_macro_def(struct string_split split_string, struct Macro **macro_ptr)
{
    if (strncmp(split_string.string[0], STARTMACR, strlen(STARTMACR)) == 0)
    {
        (*macro_ptr) = create_macro(split_string.string[1]);
        if ((*macro_ptr) == NULL)
        {
            return -1;
        }
        return 1;
    }
    return 0;
}

int determine_line_type(struct string_split split_result, struct Macro **macro_table, struct Macro **macro_ptr)
{
    int def_result = 0, call_result = 0, end_result = 0, body_result = 0;
    if ((def_result = is_macro_def(split_result, macro_ptr)))
    {
        return MACRO_DEF;
    }
    else if (end_result = is_macro_end(split_result, macro_ptr))
    {
        return MACRO_END;
    }
    else if (call_result = is_macro_call(split_result, macro_table, macro_ptr))
    {
        return MACRO_CALL;
    }
    else if (body_result = is_macro_body(split_result, macro_ptr))
    {
        return MACRO_BODY;
    }
    else if (def_result == -1 || call_result == -1 || end_result == -1 || body_result == -1)
    {
        return -1;
    }
    else
    {
        return REGULAR_LINE;
    }
}

int fill_am_file(FILE *am_file, FILE *as_file)
{
    struct Macro *macro_ptr = NULL;
    char line[MAX_LINE] = {0};
    int macro_counter = 0;
    int i;
    struct string_split split_result = {0};

    /*macro_table = (struct Macro **)allocateMemory(MACRO_TABLE_SIZE, sizeof(struct Macro *), CALLOC_ID);*/
    struct Macro macro_table[MACRO_TABLE_SIZE] = {0};

    while (fgets(line, MAX_LINE, as_file) != NULL)
    {
        split_result = split_string(line, SPACES);

        switch (determine_line_type(split_result, macro_table, &macro_ptr))
        {
        case MACRO_DEF:
            break;
        case MACRO_CALL:
            for (i = 0; i < macro_ptr->lines_counter; i++)
            {
                fputs(macro_ptr->context[i], am_file);
            }
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
        case -1:
            /* Error - exit and continue to next file */
            return -1;
            break;
        }
        if (line != NULL)
        {
            memset(line, 0, MAX_LINE); /* initalize line variable */
        }
    }
    return 1;
}

void macro_processing(char *file_name)
{
    int result = 0;

    /* Files define */
    FILE *as_file;
    FILE *am_file;

    /* File name define char */
    char asFileName[250] = {0};
    char amFileName[250] = {0};

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
    result = fill_am_file(am_file, as_file);

    if (result == -1)
    {
        printf("Error: Unable to create file\n");
        printf("Moving to the next file\n");
    }
}

int main()
{
    macro_processing("x");
    return 0;
}