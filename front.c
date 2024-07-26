#include "front.h"
#include <ctype.h>
#include <string.h>
#include "helpingFunction.c"
#include <stdlib.h>

static int is_number(char **str, int min_num, int max_num, int *result)
{
    char *endptr;
    long int num;
    num = strtol(*str, &endptr, DECIMAL_BASE);

    if (num == 0 && *str == endptr)
    {
        *result = 0;
        return 0;
    }

    if (num > max_num)
    {
        *result = 2;
        return 0;
    }

    if (num < min_num)
    {
        *result = 3;
        return 0;
    }

    *str = endptr;
    *result = 1;
    return num;
}

static int fill_string(struct string_split split_result, int index, struct ast *ast)
{
    int i, j, results[RESULT_ARR_SIZE] = {0}, data_size_ = 0;

    if (split_result.size <= index)
    {
        strcpy(ast->lineError, "String data is missing");
        return 0;
    }

    for (i = index; i < split_result.size; i++)
    {
        for (j = 0; j < strlen(split_result.string[i]); j++)
        {
            if ((i == index && j == 0) || (i == split_result.size - 1 && j == strlen(split_result.string[i]) - 1))
            {
                if (split_result.string[i][j] != STRING_CHAR)
                {
                    strcpy(ast->lineError, "String must start and end with \"");
                    return 0;
                }
            }
            else
                results[data_size_++] = split_result.string[i][j];
        }

        if (i != split_result.size - 1)
        {
            results[data_size_++] = SPACE_CHAR;
        }
    }

    results[data_size_++] = '\0';
    memcpy(ast->ast_options.dir.dir_options.data, results, data_size_ * sizeof(int));
    ast->ast_options.dir.dir_options.data_size = data_size_;

    return 1;
}

static int validate_numbers(struct string_split split_str, int size, struct ast *ast, int index)
{
    int i, data_size_ = 0, flag_comma = 0, flag_number = 0, num, result;
    char *concat_str = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);
    int results[RESULT_ARR_SIZE] = {0};

    /* Concat substring to single string */
    for (i = index; i < size; i++)
    {
        strcat(concat_str, split_str.string[i]);
        if (i != size - 1)
        { /* Adding space between substrings */
            strcat(concat_str, SPACE);
        }
    }

    /* If first or last char in .data is comma , */
    if (concat_str[0] == COMMA_CHAR || concat_str[strlen(concat_str) - 1] == COMMA_CHAR)
    {
        strcpy(ast->lineError, "Comma must not be at the start or end of the line");
        return 0;
    }

    while (concat_str[0] != '\0')
    {
        if (concat_str[0] == COMMA_CHAR)
        {
            if (flag_comma == 1)
            {
                strcpy(ast->lineError, "Comma must not be one after another");
                return 0;
            }

            flag_comma = 1, flag_number = 0;
            concat_str++;
        }

        else if (concat_str[0] != COMMA_CHAR && concat_str[0] != SPACE_CHAR)
        {
            if (flag_number == 1)
            {
                strcpy(ast->lineError, "Number must be separated by comma");
                return 0;
            }

            flag_number = 1, flag_comma = 0;
            num = is_number(&concat_str, MIN_NUM, MAX_NUM, &result);
            switch (result)
            {
            case 0:
                strcpy(ast->lineError, "Invalid number");
                return 0;
            case 1:
                results[data_size_++] = num;
                break;
            case 2:
                strcpy(ast->lineError, "Number is too big");
                return 0;
            case 3:
                strcpy(ast->lineError, "Number is too small");
                return 0;
            }
        }

        else
        {
            concat_str++;
        }
    }

    memcpy(ast->ast_options.dir.dir_options.data, results, data_size_ * sizeof(int));
    ast->ast_options.dir.dir_options.data_size = data_size_;

    return 1;
}

static int is_label(char *str, struct ast *ast)
{
    int i;
    int size = strlen(str);

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
        if (str[i] == LABEL_CHAR)
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

static int is_register(char *str)
{
    int result;
    if (str[0] == 'r' && is_number(str[1], 0, 7, &result) && str[2] == '\0')
    {
        return 1;
    }
    return 0;
}

static int is_instruction(char *str)
{
    int i;
    for (i = 0; i < INST_SIZE; i++)
    {
        if (strcmp(str, inst_table[i].name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

static void parse_operand(char *operand, int operand_type, struct ast *ast, struct inst *inst)
{
    /* parse the operand */
}

static void parse_operands(char *operands, struct ast *ast)
{
    ast->ast_type = ast_inst;
}

static void fill_directive_ast(struct ast *ast, struct string_split split_result, int index)
{
    ast->ast_type = ast_dir;

    if (strcmp(split_result.string[index], DIRECTIVE_DATA) == 0)
    {
        ast->ast_options.dir.dir_type = ast_data;
        if (!validate_numbers(split_result, split_result.size, ast, ++index))
        {
            ast->ast_type = ast_error;
        }
    }
    else if (strcmp(split_result.string[index], DIRECTIVE_STRING) == 0)
    {
        ast->ast_options.dir.dir_type = ast_string;
        if (!fill_string(split_result, index + 1, ast))
        {
            ast->ast_type = ast_error;
        }
    }
    else if (strcmp(split_result.string[index], DIRECTIVE_ENTRY) == 0)
    {
        ast->ast_options.dir.dir_type = ast_entry;
        if (is_label(split_result.string[index + 1], ast))
        {
            ast->ast_options.dir.dir_options.label = split_result.string[index + 1];
        }
    }
    else if (strcmp(split_result.string[index], DIRECTIVE_EXTERN) == 0)
    {
        ast->ast_options.dir.dir_type = ast_extern;
        if (is_label(split_result.string[index + 1], ast))
        {
            ast->ast_options.dir.dir_options.label = split_result.string[index + 1];
        }
    }
    else
    {
        strcpy(ast->lineError, "Invalid directive");
        ast->ast_type = ast_error;
    }

    return;
}

struct string_split split_string(char *str)
{
        int strings_count = 0;
    struct string_split split_result = {0};
    int in_quotes = 0;

    /** Skip leading whitespaces **/
    while (isspace((unsigned char)*str))
        str++;

    /** If the string is empty after removing whitespaces **/
    if (*str == '\0') {
        return split_result;
    }

    while (str && *str != '\0') {
        if(*str == '\n'){
            *str = '\0';
            break;
        }

        /** Check for the start or end of a quoted substring **/
        if (*str == '\"') {
            in_quotes = !in_quotes; /** Toggle in_quotes flag **/
        }

        /** Store current string in the list **/
        split_result.string[strings_count++] = str;

        if (in_quotes) {
            /** Find the closing quote **/
            str = strchr(str + 1, '\"');
            if (str) {
                str++; /** Move past the closing quote **/
            } else {
                break; /** No closing quote found **/
            }
        } else {
            /** Move to the next string **/
            str = strpbrk(str, SPACES);

            if (str) {
                *str = '\0'; /** Null terminate **/
                str++;

                /** Skip additional whitespaces **/
                while (isspace((unsigned char)*str)) {
                    str++;
                }
            }
        }
    }

    /** Update strings counter in the list **/
    split_result.size = strings_count;
    /** Return string_split structure **/
    return split_result;
}

struct ast get_ast_from_line(char *line)
{
    struct ast ast = {0}; /* Init ast type */
    int index = 0;        /* index init */
    struct string_split split_result = split_string(line);

    /* Empty line case */
    if (split_result.size == 0)
    {
        ast.ast_type = ast_empty;
        return ast;
    }

    /* Comment line case */
    if (split_result.string[0][0] == COMMENT_CHAR)
    {
        ast.ast_type = ast_comment;
        return ast;
    }

    /* If current line is directive line with . without label at start */
    if (split_result.string[index][0] == DIRECTIVE_CHAR)
    {
        fill_directive_ast(&ast, split_result, index);
        return ast;
    }

    /* If current line is instruction line without a label at start */
    if (is_instruction(split_result.string[index]))
    {
        parse_operands(split_result.string[index], &ast);
        return ast;
    }

    /* Directive line case (data, string, entry, extern) */
    /* OR Instruction line case with label at start */
    if (is_label(split_result.string[index], &ast))
    {
        ast.labelName = split_result.string[index++];    /* Init label name */
        ast.labelName[strlen(ast.labelName) - 1] = '\0'; /* Remove the ':' from the label name */

        /* If current line is directive line with .*/
        if (split_result.string[index][0] == DIRECTIVE_CHAR)
        {
            fill_directive_ast(&ast, split_result, index);
            return ast;
        }

        /* If current line is instruction line */
        parse_operands(split_result.string[index], &ast);
        return ast;
    }

    /* Error case */
    ast.ast_type = ast_error;
    return ast;
}

int main()
{
    int i;
    char line[100];
    strcpy(line, ".string \"hello, world\"");

    FILE *file;
    file = fopen("x.am", "r");
    if (file == NULL)
    {
        printf("Error: File not found\n");
        return NULL;
    }

    while (fgets(line, MAX_LINE, file) != NULL)
    {
        struct ast ast = get_ast_from_line(line);
        printf("Line type: %d\n", ast.ast_type);
        printf("Line error: %s\n", ast.lineError);
        printf("Label name: %s\n", ast.labelName);
        printf("Directive type: %d\n", ast.ast_options.dir.dir_type);
        printf("Data size: %d\n", ast.ast_options.dir.dir_options.data_size);
        printf("Directive label: %s\n", ast.ast_options.dir.dir_options.label);
        for (i = 0; i < ast.ast_options.dir.dir_options.data_size; i++)
        {
            printf("Data[%d]: %d\n", i, ast.ast_options.dir.dir_options.data[i]);
        }
    }

    return 0;
}