#include "helpingFunction.h"

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
        printf("Error: Unable to allocate memory\n");
        exit(0);
        /*continue to next file*/
    }
    return ptr;
}

/* definition int is 1 if definition label else 0*/
int is_label(char const *str, struct ast *ast, int const definition)
{
    int i;
    int size;

    if(!str){
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
    return 0;
}

int is_instruction(char const *str, struct ast *ast)
{
    int i;
    for (i = 0; i < INST_SIZE; i++)
    {
        if (strcmp(str, inst_table[i].name) == 0)
        {
            ast->ast_type = ast_inst;
            ast->ast_options.inst.inst_type = inst_table[i].opcode;
            return 1;
        }
    }
    return 0;
}
