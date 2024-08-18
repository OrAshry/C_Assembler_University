#include "helpingFunction.h"

/* Saved words array for use in other files */
char *saved_words[] = {"mov", "cmp", "add", "sub",
                       "not", "clr", "lea", "inc",
                       "dec", "jmp", "bne", "red",
                       "prn", "jsr", "rts", "stop",
                       "data", "string", "entry", "extern"};

/**
 * @brief Allocates memory using a specified allocation function.
 *
 * @param numElements Number of elements to allocate.
 * @param sizeOfElement Size of each element.
 * @param functionID The ID specifying the allocation function (CALLOC, MALLOC, REALLOC).
 * @return void* Pointer to the allocated memory, or NULL if allocation fails.
 */
void *allocateMemory(size_t numElements, size_t sizeOfElement, int functionID)
{
    void *ptr;
    switch (functionID)
    {
    case MALLOC_ID:
        ptr = malloc(numElements * sizeOfElement);
        break;
    case CALLOC_ID:
        ptr = calloc(numElements, sizeOfElement);
        break;
    }

    if (ptr == NULL)
    {
        failureExit("Memory allocation failed");
    }
    return ptr;
}

/**
 * @brief Checks if a string is a valid label and optionally updates the AST.
 *
 * @param str The string to check.
 * @param ast A pointer to the AST structure to update if `definition` is true.
 * @param definition Indicates if this is a label definition (1) or reference (0).
 * @return int Returns 1 if the string is a valid label, otherwise 0.
 */
int is_label(char const *str, struct ast *ast, int const definition)
{
    int i;
    int size;

    if (!str)
    {
        return 0;
    }

    size = strlen(str);

    if (!isalpha(str[0]))
    {
        strcpy(ast->lineError, "Label must start with a letter");
        return 0;
    }

    if (size > MAX_LABEL_SIZE)
    {
        strcpy(ast->lineError, "Label is too long.. Not more than 31 chars.");
        return 0;
    }

    for (i = 1; str[i] != '\0'; i++)
    {
        if (str[i] == LABEL_CHAR && definition)
        {
            return 1;
        }
        if (!isalpha(str[i]) && !isdigit(str[i]))
        {
            strcpy(ast->lineError, "Label must contain only letters and digits");
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Check if a string is register from r0 to r7
 * @param str The string to check
 * @return 1 if the string is a register, 0 otherwise
 */
int is_register(char const *str)
{
    char *endptr = NULL;
    int result;
    char check_str[2];
    check_str[0] = str[1];
    check_str[1] = '\0';

    if (str[0] == REGISTER_CHAR && is_number(check_str, REGISTER_MIN, REGISTER_MAX, &result, &endptr) && endptr && str[2] == '\0')
    {
        return 1;
    }

    if (strcmp(str, "r0") == 0)
    { /* Special case - if register is r0, is_number return 0 */
        return 1;
    }
    return 0;
}

/**
 * @brief Checks if a given string matches a known instruction and updates the AST accordingly.
 *
 * @param str The string to be checked as a potential instruction.
 * @param ast A pointer to the abstract syntax tree structure to be updated if the instruction is found.
 *
 * @return int Returns 1 if the string matches a known instruction, otherwise returns 0.
 */
int is_instruction(char const *str, struct ast *ast)
{
    int i;
    for (i = 0; i < INST_SIZE; i++)
    {
        if (strcmp(str, inst_table[i].name) == 0)
        {
            ast->ast_type = ast_inst;
            ast->ast_options.inst.inst_type = inst_table[i].opcode;
            ast->ast_options.inst.operands[0].operand_type = ast_none;
            ast->ast_options.inst.operands[1].operand_type = ast_none;
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Prints an error message and exits the program with a failure status.
 *
 * @param message The error message to be printed before exiting.
 */
void failureExit(char *message)
{
    printf("%s\n", message);
    exit(1);
}

/**
 * @brief Checks if the given string matches any saved word.
 *
 * @param str The string to be checked.
 *
 * @return int Returns 1 if the string is a saved word, otherwise returns 0.
 */
int is_saved_word(char const *str)
{
    int i;
    for (i = 0; i < SAVE_WORDS_SIZE; i++)
    {
        if (strcmp(str, saved_words[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}
