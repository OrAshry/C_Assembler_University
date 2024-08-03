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

    /* Check data is defined in .data */
    if (concat_str[0] == NULL_BYTE)
    {
        strcpy(ast->lineError, "Data is missing");
        return 0;
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

int get_operand_type(char *operand, struct ast *ast)
{
    if (operand[0] == '#' || operand[0] == '*')
    {
        return ((operand[0] == '#') ? ast_immidiate : ast_register_address);
    }
    else if (is_register(operand))
    {
        return ast_register_direct;
    }
    else if (is_label(operand, ast, NOT_DEFINITION_LABEL))
    {
        return ast_label;
    }
    else
    {
        strcpy(ast->lineError, "Instruction line syntax error");
        ast->ast_type = ast_error;
        return -1;
    }
}

void update_ast_operands(char *value, struct ast *ast, int operand_type, int operand_index)
{
    switch (operand_type)
    {
    case ast_immidiate:
        ast->ast_options.inst.operands[operand_index].operand_type = ast_immidiate;
        ast->ast_options.inst.operands[operand_index].operand_option.immed = atoi(value + 1);
        break;
    case ast_register_direct:
        ast->ast_options.inst.operands[operand_index].operand_type = ast_register_direct;
        ast->ast_options.inst.operands[operand_index].operand_option.reg = atoi(value + 1);
        break;
    case ast_register_address:
        ast->ast_options.inst.operands[operand_index].operand_type = ast_register_address;
        ast->ast_options.inst.operands[operand_index].operand_option.reg = atoi(value + 2);
        break;
    case ast_label:
        ast->ast_options.inst.operands[operand_index].operand_type = ast_label;
        strcpy(ast->ast_options.inst.operands[operand_index].operand_option.label, value);
        break;
    default:
        break;
    }
}

void set_ast_inst_two_operands(struct ast *ast, struct string_split split_result, int index)
{
    struct inst inst = inst_table[ast->ast_options.inst.inst_type];
    char line_error[MAX_LINE] = {0};
    int source_type = get_operand_type(split_result.string[0], ast);
    int dest_type = get_operand_type(split_result.string[1], ast);

    if (is_op_valid(source_type, inst.source) == 0)
    {
        strcat(line_error, "Invalid source operand type in ");
        strcat(line_error, inst.name);
        strcat(line_error, " instruction");
        strcpy(ast->lineError, line_error);
        ast->ast_type = ast_error;
        return;
    }

    if (is_op_valid(dest_type, inst.dest) == 0)
    {
        strcat(line_error, "Invalid dest operand type in ");
        strcat(line_error, inst.name);
        strcat(line_error, " instruction");
        strcpy(ast->lineError, line_error);
        ast->ast_type = ast_error;
        return;
    }

    update_ast_operands(split_result.string[0], ast, source_type, 0); /* Update source operand */
    update_ast_operands(split_result.string[1], ast, dest_type, 1);   /* Update destination operand*/
}

void set_ast_inst_one_operands(struct ast *ast, struct string_split split_result, int index)
{
    struct inst inst = inst_table[ast->ast_options.inst.inst_type];
    char line_error[MAX_LINE] = {0};
    int dest_type = get_operand_type(split_result.string[0], ast);

    if (is_op_valid(dest_type, inst.dest) == 0)
    {
        strcat(line_error, "Invalid dest operand type in ");
        strcat(line_error, inst.name);
        strcat(line_error, " instruction");
        strcpy(ast->lineError, line_error);
        ast->ast_type = ast_error;
        return;
    }

    update_ast_operands(split_result.string[0], ast, dest_type, 0); /* Update destination operand */
}

void parse_operands(struct string_split operands, int index, struct ast *ast)
{
    struct string_split temp_split_str = {0};
    struct inst inst = inst_table[ast->ast_options.inst.inst_type];
    char *concat_string = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);
    int i = 0;

    /* Concat string */
    for (i = index; i < operands.size; i++)
    {
        strcat(concat_string, operands.string[i]);
        if (i != operands.size - 1)
        {
            strcat(concat_string, SPACE);
        }
    }

    /* If comma in string, we need to split string by comma */
    if ((operands.size > index) && strchr(operands.string[index], COMMA_CHAR) != NULL)
    {
        temp_split_str = split_string(concat_string, COMMA);
    }

    /* If comma not in string, split by SPACE */
    if (!temp_split_str.size)
        temp_split_str = split_string(concat_string, SPACE);

    /* Case of two operands in instruction */
    if (inst.source[0] && inst.dest[0])
    {
        if (temp_split_str.size != 2)
        {
            strcpy(ast->lineError, "Invalid number of operands");
            ast->ast_type = ast_error;
            return;
        }
        set_ast_inst_two_operands(ast, temp_split_str, index);
    }

    /* Case of one destination operand in instruction */
    else if (inst.dest[0])
    {
        if (temp_split_str.size != 1)
        {
            strcpy(ast->lineError, "Invalid number of operands");
            ast->ast_type = ast_error;
            return;
        }
        set_ast_inst_one_operands(ast, temp_split_str, index);
    }

    /* Case of none operands in instruction */
    else
    {
        if (temp_split_str.size != 0)
        {
            strcpy(ast->lineError, "Invalid number of operands");
            ast->ast_type = ast_error;
            return;
        }
        /* None action needed cause no operands in these instruction type */
    }

    free(concat_string);
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
        if (is_label(split_result.string[index + 1], ast, NOT_DEFINITION_LABEL))
        {
            strcpy(ast->ast_options.dir.dir_options.label, split_result.string[index + 1]);
        }
        else
        {
            strcpy(ast->lineError, "Missing label after .entry / .extern");
            ast->ast_type = ast_error;
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
    if (*str == NULL_BYTE)
        return split_result;

    /*while (str && *str != '\0')*/
    while (str)
    {
        /** If string final char is enter **/
        if (*str == '\n')
        {
            *str = NULL_BYTE;
            break;
        }

        /** Check for the start or end of a quoted substring **/
        if (*str == '\"')
            in_quotes = !in_quotes; /** Toggle in_quotes flag **/

        /** Store current string in the list **/
        if (*str != NULL_BYTE)
        {
            split_result.string[strings_count++] = str;
        }

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
                *str = NULL_BYTE; /** Null terminate **/
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
    struct ast ast = {0};  /* Init ast type */
    int index = 0, result; /* index init */

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
    if (is_label(split_result.string[index], &ast, DEFINITION_LABEL) && !is_instruction(split_result.string[index], &ast))
    {
        strcpy(ast.labelName, split_result.string[index++]); /* Init label name */

        /* Check label syntax is correct - end with : */
        if (ast.labelName[strlen(ast.labelName) - 1] != LABEL_CHAR)
        {
            ast.ast_type = ast_error;
            strcpy(ast.lineError, "Label definition must end with ':'");
            return ast;
        }

        ast.labelName[strlen(ast.labelName) - 1] = NULL_BYTE; /* Remove the ':' from the label name */
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
    if (ast.lineError[0] != NULL_BYTE)
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
    char line[81];

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
        if (ast.ast_type == ast_empty || ast.ast_type == ast_comment)
        {
            printf("comment line or empty line\n");
            continue;
        }

        else if (ast.ast_type == ast_error)
        {
            printf("Error: %s\n", ast.lineError);
            continue;
        }

        else if (ast.labelName[0] != '\0')
        {
            printf("Label name: %s\n", ast.labelName);
        }

        if (ast.ast_type == ast_dir)
        {
            printf("Directive type: %d\n", ast.ast_options.dir.dir_type);
            printf("Directive label: %s\n", ast.ast_options.dir.dir_options.label);
            printf("Data size: %d\n", ast.ast_options.dir.dir_options.data_size);
            for (i = 0; i < ast.ast_options.dir.dir_options.data_size; i++)
            {
                printf("Data[%d]: %d\n", i, ast.ast_options.dir.dir_options.data[i]);
            }
        }

        if (ast.ast_type == ast_inst)
        {
            printf("Instruction type: %d\n", ast.ast_options.inst.inst_type);
            for (i = 0; i < 2; i++)
            {
                printf("Operand type[%d]: %d\n", i, ast.ast_options.inst.operands[i].operand_type);
                printf("Operand option[%d]: %d\n", i, ast.ast_options.inst.operands[i].operand_option.immed);
                printf("Operand option[%d]: %s\n", i, ast.ast_options.inst.operands[i].operand_option.label);
                printf("Operand option[%d]: %d\n", i, ast.ast_options.inst.operands[i].operand_option.reg);
            }
        }
    }

    return 0;
}
