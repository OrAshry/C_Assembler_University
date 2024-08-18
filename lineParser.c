#include "lineParser.h"

struct inst inst_table[INST_SIZE] = {
    {"mov", 0, "0123", "123"},
    {"cmp", 1, "0123", "0123"},
    {"add", 2, "0123", "123"},
    {"sub", 3, "0123", "123"},
    {"lea", 4, "1", "123"},
    {"clr", 5, "", "123"},
    {"not", 6, "", "123"},
    {"inc", 7, "", "123"},
    {"dec", 8, "", "123"},
    {"jmp", 9, "", "12"},
    {"bne", 10, "", "12"},
    {"red", 11, "", "123"},
    {"prn", 12, "", "0123"},
    {"jsr", 13, "", "12"},
    {"rts", 14, "", ""},
    {"stop", 15, "", ""}};

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

    if (end_ptr)
    {
        *end_ptr = endptr;
    }

    *result = 1;
    return num;
}

int is_defined_macro(char *label, struct MacroContext *macro_table)
{
    int i;
    for (i = 0; i < macro_table->macro_counter; i++)
    {
        if (strcmp(label, macro_table->macro_table[i]->name) == 0)
        {
            return 1;
        }
    }
    return 0;
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
    int result = __INT_MAX__;
    int integer_value;
    switch (operand_type)
    {
    case ast_immidiate:
        ast->ast_options.inst.operands[operand_index].operand_type = ast_immidiate;
        integer_value = is_number(value + 1, MIN_NUM_IMMID, MAX_NUM_IMMID, &result, NULL);
        ast->ast_options.inst.operands[operand_index].operand_option.immed = integer_value;
        break;
    case ast_register_direct:
        ast->ast_options.inst.operands[operand_index].operand_type = ast_register_direct;
        integer_value = is_number(value + 1, MIN_NUM, MAX_NUM, &result, NULL);
        ast->ast_options.inst.operands[operand_index].operand_option.reg = integer_value;
        break;
    case ast_register_address:
        ast->ast_options.inst.operands[operand_index].operand_type = ast_register_address;
        integer_value = is_number(value + 2, MIN_NUM, MAX_NUM, &result, NULL);
        ast->ast_options.inst.operands[operand_index].operand_option.reg = integer_value;
        break;
    case ast_label:
        ast->ast_options.inst.operands[operand_index].operand_type = ast_label;
        strcpy(ast->ast_options.inst.operands[operand_index].operand_option.label, value);
        break;
    default:
        break;
    }

    switch (result)
    {
    case 0:
        strcpy(ast->lineError, "Invalid number");
        ast->ast_type = ast_error;
        break;
    case 2:
        strcpy(ast->lineError, "Number is too big");
        ast->ast_type = ast_error;
        break;
    case 3:
        strcpy(ast->lineError, "Number is too small");
        ast->ast_type = ast_error;
        break;
    default:
        break;
    }
}

void set_ast_inst_two_operands(struct ast *ast, struct string_split split_result)
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

void set_ast_inst_one_operands(struct ast *ast, struct string_split split_result)
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

char *concat_string_split(struct string_split split_result, int const index, int const size)
{
    char *concat_string = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);
    int i = 0;

    for (i = index; i < size; i++)
    {
        strcat(concat_string, split_result.string[i]);
        if (i != size - 1)
        {
            strcat(concat_string, SPACE);
        }
    }

    return concat_string;
}

/*
Check if comma between operands
@param string - the string to check
@param first_operand - the first operand
@return 1 if comma between operands, 0 if not, -1 if error
*/
int has_comma_between_operands(char *string, char *first_operand)
{
    const char *ptr = strstr(string, first_operand);
    int index, i;

    if (ptr != NULL) {
        index = ptr - string; /* Index of first_operand in string */
    } else {
        return -1;
    }

    for(i = index; i < strlen(string); i++){
        if(string[i] == COMMA_CHAR){
            return 1;
        }
    }
    return 0;
}

void parse_operands(struct string_split operands, int index, struct ast *ast)
{
    struct string_split temp_split_str = {0};
    struct inst inst = inst_table[ast->ast_options.inst.inst_type];
    char *concat_string = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);
    char *original_concat_string = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);
    char *temp_concat;
    int i = 0;

    /* Concat string */
    strcpy(concat_string, concat_string_split(operands, index, operands.size));
    strcpy(original_concat_string, concat_string);

    /* If comma in string and instruction only source and dest operands, we need to split string by comma */
    if ((operands.size > index) && strchr(concat_string, COMMA_CHAR) != NULL && inst.source[0] && inst.dest[0])
    {
        /* First part: Split by comma */
        temp_split_str = split_string(concat_string, COMMA);

        /* Second part: Remove spaces if exists (just in case) */
        if (temp_split_str.size == 2)
        {
            temp_concat = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID); /* Init temp_concat */
            strcpy(temp_concat, concat_string_split(temp_split_str, 0, temp_split_str.size));

            for (i = 0; i < temp_split_str.size; i++)
            {
                memset(temp_split_str.string[i], 0, strlen(temp_split_str.string[i]));
            }
            temp_split_str.size = 0;

            temp_split_str = split_string(temp_concat, SPACES);

            memset(concat_string, 0, strlen(concat_string));
            strcpy(concat_string, temp_concat);
            free(temp_concat);
        }
    }

    /* If comma not in string, split by SPACE */
    if (!temp_split_str.size)
        temp_split_str = split_string(concat_string, SPACE);

    if (index < operands.size && operands.string[index][0] == COMMA_CHAR)
    {
        strcpy(ast->lineError, "Comma must not come after the instruction");
        ast->ast_type = ast_error;
        return;
    }
    else if (operands.string[operands.size - 1][0] == COMMA_CHAR)
    {
        strcpy(ast->lineError, "Comma must not be at the end of the line");
        ast->ast_type = ast_error;
        return;
    }

    /* Case of two operands in instruction */
    if (inst.source[0] && inst.dest[0])
    {
        if (temp_split_str.size != 2)
        {
            strcpy(ast->lineError, "Instruction must have two operands while separated by comma");
            ast->ast_type = ast_error;
            return;
        }
        
        if ((index + 1) < operands.size && !has_comma_between_operands(original_concat_string, temp_split_str.string[0]))
        {
            strcpy(ast->lineError, "Comma must be between operands");
            ast->ast_type = ast_error;
            return;
        }
        set_ast_inst_two_operands(ast, temp_split_str);
    }

    /* Case of one destination operand in instruction */
    else if (inst.dest[0])
    {
        if (temp_split_str.size != 1)
        {
            strcpy(ast->lineError, "Instruction must have one operand only");
            ast->ast_type = ast_error;
            return;
        }
        set_ast_inst_one_operands(ast, temp_split_str);
    }

    /* Case of none operands in instruction */
    else
    {
        if (temp_split_str.size != 0)
        {
            strcpy(ast->lineError, "Instruction must have no operands");
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
            ast->ast_type = ast_error;
        }
    }
    else
    {
        strcpy(ast->lineError, "Invalid directive");
        ast->ast_type = ast_error;
    }
}

struct ast get_ast_from_line(char *line, struct MacroContext *macro_table)
{
    struct ast ast = {0}; /* Init ast type */
    int index = 0;        /* index init */

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
    if (split_result.string[index][0] != DIRECTIVE_CHAR &&
        is_label(split_result.string[index], &ast, DEFINITION_LABEL) &&
        !is_instruction(split_result.string[index], &ast))
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

        if (is_saved_word(ast.labelName))
        {
            strcpy(ast.lineError, "Label name is a saved word");
            ast.ast_type = ast_error;
            return ast;
        }
        else if (macro_table != NULL && is_defined_macro(ast.labelName, macro_table))
        {
            strcpy(ast.lineError, "Label name is a already defined as macro name");
            ast.ast_type = ast_error;
            return ast;
        }
    }

    /* If current line is directive line with . */
    if (ast.lineError[0] == NULL_BYTE && split_result.string[index][0] == DIRECTIVE_CHAR)
    {
        fill_directive_ast(&ast, split_result, index);
        return ast;
    }

    /* If current line is instruction line */
    if (ast.lineError[0] == NULL_BYTE && is_instruction(split_result.string[index++], &ast))
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
