#include "MacroProcessing.h"

FILE *open_file(char *file_name, char *mode)
{
    FILE *file = fopen(file_name, mode);
    if (file == NULL)
    {
        failureExit("Unable to open / create file");
    }
    return file;
}

int check_duplicate_macro(const char *macro_name, struct Macro **macro_table, const int macro_counter)
{
    int i;
    for (i = 0; i < macro_counter; i++)
    {
        if (macro_table[i] != NULL && strcmp(macro_table[i]->name, macro_name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

struct Macro *create_macro(char *token, int *result, struct Macro **macro_table, const int macro_counter)
{
    char *macro_name = NULL;
    struct Macro *macro_ptr = (struct Macro *)allocateMemory(1, sizeof(struct Macro), CALLOC_ID);

    macro_ptr->lines_counter = 0;
    macro_name = get_macro_name(token);
    if (macro_name == NULL)
    {
        macro_ptr = NULL;
        printf("Error: Unable to create macro because of additional data\n");
        printf("Moving to the next file\n");
        *result = -1;
        return NULL;
    }

    if(check_duplicate_macro(macro_name, macro_table, macro_counter) == 1){
        *result = -2;
        return NULL;
    }

    macro_ptr->context = (char **)allocateMemory(DEF_MAT_SIZE, sizeof(char *), CALLOC_ID);
    strcpy(macro_ptr->name, macro_name);
    if (macro_name != NULL)
    {
        free(macro_name);
    }

    *result = 1;
    return macro_ptr;
}

void update_macro_context(char *line, struct Macro **macro_ptr)
{
    char **temp_ptr = NULL;

    if (macro_ptr == NULL) return;

    if ((*macro_ptr)->lines_counter >= DEF_MAT_SIZE)
    {
        temp_ptr = (char **)realloc((*macro_ptr)->context, ((*macro_ptr)->lines_counter + 1) * sizeof(char *));
        if (*temp_ptr == NULL)
        {
            *macro_ptr = NULL;
            failureExit("Unable to allocate memory");
        }
        (*macro_ptr)->context = temp_ptr;
    }

    (*macro_ptr)->context[(*macro_ptr)->lines_counter] = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);

    while (isspace(*line))
    {
        line++;
    }
    strcpy((*macro_ptr)->context[(*macro_ptr)->lines_counter++], line);
}

char *get_macro_name(char *token)
{
    char *name = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);

    /* If current row is empty */
    if (strcmp(token, "\n") == 0)
    {
        return NULL;
    }

    strcpy(name, token);

    token = strtok(NULL, " ");
    if (token != NULL)
    {
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
    struct Macro *temp_macro = NULL;

    if (macro_ptr == NULL)
    {
        return;
    }

    if (macro_counter >= MACRO_TABLE_SIZE)
    {
        temp_ptr = (struct Macro *)realloc(*macro_table, (macro_counter + 1) * sizeof(struct Macro *));
        if (temp_ptr == NULL)
        {
            failureExit("Unable to allocate memory");
        }
        *macro_table = temp_ptr;
    }

    temp_macro = (struct Macro *)allocateMemory(1, sizeof(struct Macro), CALLOC_ID);

    memcpy(temp_macro, macro_ptr, sizeof(struct Macro));
    macro_table[macro_counter] = temp_macro;
}

int is_macro_def(char *line, struct Macro **macro_ptr, struct Macro **macro_table, const int macro_counter)
{
    int result;

    char *token = NULL, temp_line[MAX_LINE] = {0};
    if (*macro_ptr != NULL)
    {
        return 0;
    }

    strcpy(temp_line, line);
    token = strtok(temp_line, " ");
    if (strncmp(token, STARTMACR, strlen(STARTMACR)) == 0)
    {
        token = strtok(NULL, " ");
        (*macro_ptr) = create_macro(token, &result, macro_table, macro_counter);
        return result;
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
    char temp_line[MAX_LINE] = {0};

    if (*macro_ptr != NULL)
    {
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

    return 0;
}

int is_macro_end(char *line, struct Macro **macro_ptr)
{
    char *token = NULL;
    char temp_line[MAX_LINE] = {0};

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

int determine_line_type(char *line, struct Macro **macro_table, struct Macro **macro_ptr, const int macro_counter)
{
    int def_result, body_result, call_result, end_result;
    if ((def_result = is_macro_def(line, macro_ptr, macro_table, macro_counter)) == 1)
    {
        return MACRO_DEF;
    }
    else if ((end_result = is_macro_end(line, macro_ptr)) == 1)
    {
        return MACRO_END;
    }
    else if ((call_result = is_macro_call(line, macro_table, macro_ptr)) == 1)
    {
        return MACRO_CALL;
    }
    else if ((body_result = is_macro_body(line, macro_ptr)) == 1)
    {
        return MACRO_BODY;
    }
    else if (def_result == -1 || body_result == -1 || call_result == -1 || end_result == -1)
    {
        return -1;
    }
    else if (def_result == -2)
    {
        return -2;
    }
    else
    {
        return REGULAR_LINE;
    }
}

struct MacroContext fill_am_file(FILE *am_file, FILE *as_file, int *result, int *macro_counter)
{
    struct Macro *macro_ptr = NULL;
    struct MacroContext macro_context = {NULL, 0};
    char line[MAX_LINE] = {0};
    int mcr_counter = 0, i;
    struct Macro **macro_table = (struct Macro **)allocateMemory(MACRO_TABLE_SIZE, sizeof(struct Macro *), CALLOC_ID);

    while (fgets(line, MAX_LINE, as_file) != NULL)
    {
        switch (determine_line_type(line, macro_table, &macro_ptr, mcr_counter))
        {
        case MACRO_DEF:
            break;
        case MACRO_CALL:
            for (i = 0; i < macro_ptr->lines_counter; i++)
            {
                fputs(macro_ptr->context[i], am_file);
            }
            macro_ptr = NULL;
            break;
        case MACRO_END:
            append_macro_table(macro_table, macro_ptr, mcr_counter);
            mcr_counter++;
            macro_ptr = NULL;
            break;
        case REGULAR_LINE:
            fputs(line, am_file);
            break;
        case MACRO_BODY:
            break;
        case -1:
            printf("Continue to next file because of error!\n");
            *result = -1;
            mcr_counter--;
            *macro_counter = mcr_counter;
            return macro_context;
            break;
        case -2:
            printf("Continue to next file because of error!\n");
            *result = -2;
            mcr_counter--;
            *macro_counter = mcr_counter;
            return macro_context;
            break;
        }
        if (line != NULL)
        {
            memset(line, 0, MAX_LINE); /* initalize line variable */
        }
    }

    if (macro_ptr != NULL)
    {
        free(macro_ptr);
    }

    *result = 0;
    macro_context.macro_table = macro_table;
    macro_context.macro_counter = mcr_counter;
    return macro_context; 
}

void free_macro_table(struct MacroContext *macro_table) {
    int i, j;
    for (i = 0; i < macro_table->macro_counter; i++) {
        for (j = 0; j < macro_table->macro_table[i]->lines_counter; j++) {
            free(macro_table->macro_table[i]->context[j]);
        }
        free(macro_table->macro_table[i]->context);
        free(macro_table->macro_table[i]);
    }
    free(macro_table->macro_table);
}

void print_macro_table(struct MacroContext *macro_table) {
    int i, j;
    for (i = 0; i < macro_table->macro_counter; i++) {
        printf("Macro name: %s\n", macro_table->macro_table[i]->name);
        for (j = 0; j < macro_table->macro_table[i]->lines_counter; j++) {
            printf("Macro context: %s\n", macro_table->macro_table[i]->context[j]);
        }
    }
}

void macro_processing(char *file_name, struct MacroContext *macro_table)
{
    int result, macro_counter = 0;

    /* Files define */
    FILE *as_file;
    FILE *am_file;

    /* File name define char */
    char *asFileName;
    char *amFileName;

    /* Allocate data memory */
    asFileName = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);
    amFileName = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);

    /* Copy read file name with ending */
    strcpy(asFileName, file_name);
    strcat(asFileName, ".as");

    /* Copy write file name with ending */
    strcpy(amFileName, file_name);
    strcat(amFileName, ".am");

    /* Read & Create file */
    as_file = open_file(asFileName, "r");
    am_file = open_file(amFileName, "w");

    /* Creating new am file and getting macro table */
    *macro_table = fill_am_file(am_file, as_file, &result, &macro_counter);

    /* Check for error - delete file */
    if (result == -1)
    {
        if (remove(amFileName) == 0)
        {
            printf("File deleted successfully\n");
        }
    }else if(result == -2){
        printf("Error: Duplicate in macro defining with same name!\n");
    }

    /* Free macro table */
    print_macro_table(macro_table);
    free_macro_table(macro_table);

    /* Close files */
    if (as_file != NULL)
        fclose(as_file);
    if (am_file != NULL)
        fclose(am_file);

    /* Free memory */
    free(asFileName);
    free(amFileName);
}

int main()
{
    struct MacroContext macro_table;
    macro_processing("test_tomer", &macro_table);
    return 0;
}