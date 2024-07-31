#include "front.h"
#include <ctype.h>
#include <string.h>
#include "helpingFunction.c"
#include <stdlib.h>

int is_number(char *str, int const min_num, int const max_num, int *result, char **end_ptr)
{
    char *endptr;
    long int num = strtol(str, &endptr, DECIMAL_BASE);

    if (num == 0 && str == endptr)
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

    *end_ptr = endptr;
    *result = 1;
    return num;
}

int fill_string(struct string_split const split_result, int const index, struct ast *ast)
{
    int i, j, results[RESULT_ARR_SIZE] = {0}, data_size_ = 0, last_idx;

    /* If string section isn't defined in file */
    if (split_result.size <= index)
    {
        strcpy(ast->lineError, "String data is missing");
        return 0;
    }

    /* Check for opening " */
    if (split_result.string[index][0] != STRING_CHAR)
    {
        strcpy(ast->lineError, "String must start with \"");
        return 0;
    }

    /* Check for closing " */
    last_idx = split_result.size - 1;
    if (split_result.string[last_idx][strlen(split_result.string[last_idx]) - 1] != STRING_CHAR)
    {
        strcpy(ast->lineError, "String must end with \"");
        return 0;
    }

    for (i = index; i < split_result.size; i++)
    {
        for (j = 0; j < strlen(split_result.string[i]); j++)
        {
            if ((i == index && j == 0) || (i == split_result.size - 1 && j == strlen(split_result.string[i]) - 1))
            {
                continue;
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

int validate_numbers(struct string_split const split_str, int const size, struct ast *ast, int const index)
{
    int i, data_size_ = 0, flag_comma = 0, flag_number = 0, num, result, results[RESULT_ARR_SIZE] = {0};
    char *concat_str = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);
    char *end_ptr;

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
            num = is_number(concat_str, MIN_NUM, MAX_NUM, &result, &end_ptr);
            switch (result)
            {
            case 0:
                strcpy(ast->lineError, "Invalid number");
                return 0;
            case 1:
                concat_str = end_ptr;
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

int is_label(char const *str, struct ast *ast)
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

int is_register(char const *str)
{
    char *endptr = NULL;
    char check_str[2] = {str[1], '\0'};
    int result;
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
        return 0;
    }
}

int is_op_valid(int const operand_type, char const *inst_options)
{
    int i;
    for (i = 0; i < strlen(inst_options); i++)
    {
        if (operand_type == inst_options[i] - '0')
        {
            return 1;
        }
    }
    return 0;
}

void parse_operand(char *operand, int const operand_type, struct ast *ast, int const operand_index)
{
    struct inst inst = inst_table[ast->ast_options.inst.inst_type];
    char line_error[MAX_LINE] = {0};

    if (operand_index > 1)
    {
        strcat(line_error, "Invalid source operand type in ");
        strcat(line_error, inst.name);
        strcat(line_error, " instruction");
        strcpy(ast->lineError, line_error);
        ast->ast_type = ast_error;
        return;
    }

    if (operand_index == 0 && is_op_valid(operand_type, inst.source) == 0)
    {
        strcat(line_error, "Invalid dest operand type in ");
        strcat(line_error, inst.name);
        strcat(line_error, " instruction");
        strcpy(ast->lineError, line_error);
        ast->ast_type = ast_error;
        return;
    }

    if (operand_index == 1 && is_op_valid(operand_type, inst.dest) == 0)
    {
        strcat(line_error, "Invalid dest operand type in ");
        strcat(line_error, inst.name);
        strcat(line_error, " instruction");
        strcpy(ast->lineError, line_error);
        ast->ast_type = ast_error;
        return;
    }

    if (operand_type == ast_immidiate)
    {
        ast->ast_options.inst.operands[operand_index].operand_type = ast_immidiate;
        ast->ast_options.inst.operands[operand_index].operand_option.immed = atoi(operand + 1);
    }
    else if (operand_type == ast_register_direct)
    {
        ast->ast_options.inst.operands[operand_index].operand_type = ast_register_direct;
        ast->ast_options.inst.operands[operand_index].operand_option.reg = atoi(operand + 1);
    }
    else if (operand_type == ast_register_address)
    {
        ast->ast_options.inst.operands[operand_index].operand_type = ast_register_address;
        ast->ast_options.inst.operands[operand_index].operand_option.reg = atoi(operand + 1);
    }
    else if (operand_type == ast_label)
    {
        ast->ast_options.inst.operands[operand_index].operand_type = ast_label;
        strcpy(ast->ast_options.inst.operands[operand_index].operand_option.label, operand);
    }
}

void parse_operands(struct string_split operands, int index, struct ast *ast)
{
    struct string_split temp_split_str = {0};
    int i, operand_type, operand_index = 0;
    if (strchr(operands.string[index], COMMA_CHAR) != NULL)
    {
        temp_split_str = split_string(operands.string[index], COMMA);
    }

    if (!temp_split_str.size)
        temp_split_str = operands;

    for (i = 0; i < temp_split_str.size; i++)
    {
        if (temp_split_str.string[i][0] == '#' || temp_split_str.string[i][0] == '*')
        {
            operand_type = (temp_split_str.string[i][0] == '#') ? ast_immidiate : ast_register_address;
            parse_operand(temp_split_str.string[i], operand_type, ast, operand_index);
        }
        else if (is_register(temp_split_str.string[i]))
        {
            parse_operand(temp_split_str.string[i], ast_register_direct, ast, operand_index);
        }
        else if (is_label(temp_split_str.string[i], ast))
        {
            parse_operand(temp_split_str.string[i], ast_label, ast, operand_index);
        }
        else
        {
            strcpy(ast->lineError, "Invalid operand");
            ast->ast_type = ast_error;
            return;
        }
        operand_index++;
    }
}

void fill_directive_ast(struct ast *ast, struct string_split split_result, int index)
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
    else if (strcmp(split_result.string[index], DIRECTIVE_ENTRY) == 0 || strcmp(split_result.string[index], DIRECTIVE_EXTERN) == 0)
    {
        ast->ast_options.dir.dir_type = strcmp(split_result.string[index], DIRECTIVE_ENTRY) == 0 ? ast_entry : ast_extern;
        if (is_label(split_result.string[index + 1], ast))
        {
            strcpy(ast->ast_options.dir.dir_options.label, split_result.string[index + 1]);
        }
    }
    else
    {
        strcpy(ast->lineError, "Invalid directive");
        ast->ast_type = ast_error;
    }
}

struct string_split split_string(char *str, const char *delimiter)
{
    int strings_count = 0, in_quotes = 0;
    struct string_split split_result = {0};

    /** Skip leading whitespaces **/
    while (isspace((unsigned char)*str))
        str++;

    /** If the string is empty after removing whitespaces **/
    if (*str == '\0')
        return split_result;

    /*while (str && *str != '\0')*/
    while (str)
    {
        /** If string final char is enter **/
        if (*str == '\n')
        {
            *str = '\0';
            break;
        }

        /** Check for the start or end of a quoted substring **/
        if (*str == '\"')
            in_quotes = !in_quotes; /** Toggle in_quotes flag **/

        /** Store current string in the list **/
        split_result.string[strings_count++] = str;

        if (in_quotes)
        {
            /** Find the closing quote **/
            str = strchr(str + 1, '\"');
            if (str)
            {
                str++; /** Move past the closing quote **/
            }
            else
            {
                break; /** No closing quote found **/
            }
        }
        else
        {
            /** Move to the next string **/
            str = strpbrk(str, delimiter);

            if (str)
            {
                *str = '\0'; /** Null terminate **/
                str++;

                /** Skip additional whitespaces **/
                while (isspace((unsigned char)*str))
                    str++;
            }
        }
    }

    /** Update strings counter in the list **/
    split_result.size = strings_count;

    /* Remove '\n' from end of string if exists */
    if (split_result.string[strings_count - 1][strlen(split_result.string[strings_count - 1]) - 1] == '\n')
    {
        split_result.string[strings_count - 1][strlen(split_result.string[strings_count - 1]) - 1] = '\0';
    }

    /** Return string_split structure **/
    return split_result;
}

struct ast get_ast_from_line(char *line)
{
    struct ast ast = {0};                                          /* Init ast type */
    int index = 0, result;                                         /* index init */
    struct string_split split_result = split_string(line, SPACES); /* Split line into substrings */

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

    /* If current string is a Label */
    if (is_label(split_result.string[index], &ast) && !is_instruction(split_result.string[index], &ast))
    {
        strcpy(ast.labelName, split_result.string[index++]); /* Init label name */
        ast.labelName[strlen(ast.labelName) - 1] = '\0';     /* Remove the ':' from the label name */
    }

    /* If current line is directive line with . */
    if (split_result.string[index][0] == DIRECTIVE_CHAR)
    {
        fill_directive_ast(&ast, split_result, index);
        return ast;
    }

    /* If current line is instruction line */
    if (is_instruction(split_result.string[index++], &ast))
    {
        parse_operands(split_result, index, &ast);
        return ast;
    }

    /* First Error case */
    ast.ast_type = ast_error;
    if (ast.lineError)
    {
        return ast;
    }

    /* Second Error case */
    strcpy(ast.lineError, "Invalid directive or instruction");
    return ast;
}

int main()
{
    int i;
    char line[100];

    FILE *file;
    file = fopen("x.am", "r");
    if (file == NULL)
    {
        printf("Error: File not found\n");
        return 0;
    }

    /* create me a ast with init values */

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
        printf("Instruction type: %d\n", ast.ast_options.inst.inst_type);
        for (i = 0; i < 2; i++)
        {
            printf("Operand type[%d]: %d\n", i, ast.ast_options.inst.operands[i].operand_type);
            printf("Operand option[%d]: %d\n", i, ast.ast_options.inst.operands[i].operand_option.immed);
            printf("Operand option[%d]: %s\n", i, ast.ast_options.inst.operands[i].operand_option.label);
            printf("Operand option[%d]: %d\n", i, ast.ast_options.inst.operands[i].operand_option.reg);
        }
    }

    return 0;
}