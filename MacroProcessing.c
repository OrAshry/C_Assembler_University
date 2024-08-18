#include "macroProcessing.h"

/**
 * @brief Opens a file with the specified mode.
 *
 * @param file_name The name of the file to open.
 * @param mode The mode in which to open the file (e.g., "r", "w").
 *
 * @return A pointer to the opened file stream.
 */
FILE *open_file(char *file_name, char *mode)
{
    FILE *file = fopen(file_name, mode);
    if (file == NULL)
    {
        failureExit("Unable to open / create file");
    }
    return file;
}

/**
 * @brief Checks if a macro with the specified name already exists in the macro table.
 *
 * @param macro_name The name of the macro to check for duplicates.
 * @param macro_table The array of pointers to macro structures.
 * @param macro_counter The number of macros in the macro table.
 *
 * @return 1 if a duplicate macro is found, 0 otherwise.
 */
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

/**
 * @brief Creates a new macro structure and initializes it based on the provided token.
 *
 * @param token The token from which to extract the macro name.
 * @param result Pointer to an integer that will be set to indicate the outcome of the macro creation.
 * @param macro_table Array of pointers to existing macro structures.
 * @param macro_counter The number of macros currently in the macro table.
 *
 * @return A pointer to the newly created macro structure, or NULL if the creation fails.
 */
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

    if (check_duplicate_macro(macro_name, macro_table, macro_counter) == 1)
    {
        *result = -2;
        return NULL;
    }

    if (is_saved_word(macro_name) == 1)
    {
        *result = -3;
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

/**
 * @brief Updates the context of a macro by adding a new line to it.
 *
 * @param line The line to be added to the macro's context.
 * @param macro_ptr Pointer to the macro structure to be updated.
 *
 * @return void
 */
void update_macro_context(char *line, struct Macro **macro_ptr)
{
    char **temp_ptr = NULL;

    if (macro_ptr == NULL)
        return;

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

/**
 * @brief Extracts the macro name from a given token.
 *
 * @param token The token from which the macro name is extracted.
 *
 * @return char* The extracted macro name or NULL if the token is empty or contains additional data.
 */
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
    if (token != NULL) /* This is not a macro call */
    {
        return NULL;
    }

    if (name[strlen(name) - 1] == '\n')
    {
        name[strlen(name) - 1] = '\0';
    }

    return name;
}

/**
 * @brief Appends a macro to the macro table.
 *
 * @param macro_table Pointer to the macro table.
 * @param macro_ptr Pointer to the macro to be appended.
 * @param macro_counter Current count of macros in the table.
 */
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

/**
 * @brief Checks if a line defines a macro and creates a macro if it does.
 *
 * @param line The line of text to check.
 * @param macro_ptr Pointer to the macro pointer to be updated.
 * @param macro_table Pointer to the macro table.
 * @param macro_counter Current count of macros in the table.
 * @return 1 if a macro is defined, -1 if there is additional data, -2 if the macro is duplicated,
 *         -3 if the macro name is a saved word, and 0 otherwise.
 */
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

/**
 * @brief Adds a line to the body of a macro if the macro is currently being defined.
 *
 * @param line The line of text to add to the macro body.
 * @param macro_ptr Pointer to the macro being updated.
 * @return 1 if the line was added successfully, 0 if no macro is being defined.
 */
int is_macro_body(char *line, struct Macro **macro_ptr)
{
    if (*macro_ptr == NULL)
    {
        return 0;
    }

    update_macro_context(line, macro_ptr);
    return 1;
}

/**
 * @brief Checks if the given line is a macro call and updates the macro pointer if it is.
 * 
 * @param line The line of text to check for a macro call.
 * @param macro_table Pointer to the macro table containing defined macros.
 * @param macro_ptr Pointer to the macro that matches the call, if found.
 * @param macro_counter Number of macros in the macro table.
 * @return 1 if a macro call was found and the macro pointer was updated, 0 otherwise.
 */
int is_macro_call(char *line, struct Macro **macro_table, struct Macro **macro_ptr, const int macro_counter)
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
    for (i = 0; i < macro_counter; i++)
    {
        if (strcmp(macro_table[i]->name, macro_name) == 0)
        {
            *macro_ptr = macro_table[i];
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Checks if a line indicates the end of a macro definition.
 *
 * @param line A pointer to the line to be checked.
 * @param macro_ptr A pointer to a pointer to a `Macro` structure (unused).
 *
 * @return 1 if the end-of-macro directive is correctly defined, 0 otherwise.
 */
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

/**
 * @brief Determines the type of a line based on macro definitions and usage.
 *
 * @param line A pointer to the line to be checked.
 * @param macro_table A pointer to a pointer to a `Macro` table.
 * @param macro_ptr A pointer to a pointer to a `Macro` structure.
 * @param macro_counter The count of macros currently defined.
 *
 * @return An integer representing the type of the line:
 *         - `MACRO_DEF` for macro definitions
 *         - `MACRO_END` for end of macro definitions
 *         - `MACRO_BODY` for macro bodies
 *         - `MACRO_CALL` for macro calls
 *         - `-1`, `-2`, or `-3` for specific error conditions
 *         - `REGULAR_LINE` for regular lines not related to macros.
 */
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
    else if ((body_result = is_macro_body(line, macro_ptr)) == 1)
    {
        return MACRO_BODY;
    }
    else if ((call_result = is_macro_call(line, macro_table, macro_ptr, macro_counter)) == 1)
    {
        return MACRO_CALL;
    }
    else if (def_result == -1)
    {
        return -1;
    }
    else if (def_result == -2)
    {
        return -2;
    }
    else if (def_result == -3)
    {
        return -3;
    }
    else
    {
        return REGULAR_LINE;
    }
}

/**
 * @brief Processes an assembly file and fills a macro file based on macro definitions and calls.
 *
 * @param am_file A pointer to the file where the processed macro content is written.
 * @param as_file A pointer to the file containing the macro definitions and calls.
 * @param result A pointer to an integer where the function stores the result status:
 *               - `0` for success
 *               - `-1`, `-2`, or `-3` for specific error conditions.
 * @param macro_counter A pointer to an integer where the function stores the count of macros.
 *
 * @return A `MacroContext` structure containing:
 *         - `macro_table`: A table of macros processed.
 *         - `macro_counter`: The count of macros processed.
 */
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
            *result = -1;
            break;
        case -2:
            *result = -2;
            break;
        case -3:
            *result = -3;
            break;
        }

        if(*result == -1 || *result == -2 || *result == -3)
        {
            mcr_counter--;
            *macro_counter = mcr_counter;
            return macro_context;
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

/**
 * @brief Frees the memory allocated for the macro table and its contents.
 *
 * @param macro_table A pointer to a `MacroContext` structure containing the macro table to be freed.
 */
void free_macro_table(struct MacroContext *macro_table)
{
    int i, j;
    for (i = 0; i < macro_table->macro_counter; i++)
    {
        for (j = 0; j < macro_table->macro_table[i]->lines_counter; j++)
        {
            free(macro_table->macro_table[i]->context[j]);
        }
        macro_table->macro_table[i]->context = NULL;
        macro_table->macro_table[i] = NULL;
    }
    macro_table->macro_table = NULL;
    macro_table->macro_counter = 0;
    free(macro_table->macro_table);
}

char *macro_processing(char *file_name, struct MacroContext *macro_table)
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
    if (result == -1 || result == -2 || result == -3)
    {
        if (remove(amFileName) == 0)
        {
            printf("File deleted successfully\n");
        }
        if (result == -1)
        {
            printf("Error: Unable to create macro because of additional data\n");
        }
        else if (result == -2)
        {
            printf("Error: Duplicate macro name\n");
        }
        else if (result == -3)
        {
            printf("Error: Macro call can't be saved word\n");
        }
        amFileName = NULL;
    }

    /* Close files */
    if (as_file != NULL)
        fclose(as_file);
    if (am_file != NULL)
        fclose(am_file);

    /* Free memory */
    free(asFileName);

    /* Return file name */
    return amFileName;
}
