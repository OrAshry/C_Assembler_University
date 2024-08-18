#include "lineParser.h"

/* Instructions Table init */
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

/**
 * @brief Validates if a string represents a number within a specified range.
 *
 * @param str The string to be checked as a number.
 * @param min_num The minimum allowable value for the number.
 * @param max_num The maximum allowable value for the number.
 * @param result A pointer to an integer that will store the result code:
 *               0 if the string is not a number,
 *               1 if the number is valid,
 *               2 if the number exceeds max_num,
 *               3 if the number is below min_num.
 * @param end_ptr A pointer to a string pointer where the function will store the address of the first invalid character in str (or NULL if this information is not needed).
 *
 * @return int The numeric value if valid, otherwise returns 0.
 */
int is_number(char *str, int const min_num, int const max_num, int *result, char **end_ptr)
{
    char *endptr;
    long int num = strtol(str, &endptr, DECIMAL_BASE); /* Convert string to long int */

    if (num == 0 && str == endptr) /* If string is not a number */
    {
        *result = 0;
        return 0;
    }

    if (num > max_num) /* If number is bigger than max_num */
    {
        *result = 2;
        return 0;
    }

    if (num < min_num) /* If number is smaller than min_num */
    {
        *result = 3;
        return 0;
    }

    if (end_ptr) /* If end_ptr is defined */
    {
        *end_ptr = endptr;
    }

    *result = 1; /* Number is valid */
    return num;
}

/**
 * @brief Checks if a given label matches a defined macro in the macro table.
 *
 * @param label The label to check against the macro table.
 * @param macro_table Pointer to the MacroContext structure containing the macro table and macro counter.
 * @return int Returns 1 if the label matches a defined macro, otherwise returns 0.
 */
int is_defined_macro(char *label, struct MacroContext *macro_table)
{
    int i;
    for (i = 0; i < macro_table->macro_counter; i++)
    {
        if (strcmp(label, macro_table->macro_table[i]->name) == 0) /* If label is a macro name */
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Fills the AST with string data extracted from a split string result.
 *
 * @param split_result The result of splitting a string, containing the string sections.
 * @param index The index of the section where the string data starts.
 * @param ast Pointer to the AST structure where the string data will be stored.
 *
 * @return int Returns 1 if the string data was successfully processed and stored, otherwise returns 0.
 */
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

    /* Concatenate string sections */
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

/**
 * @brief Validates and processes a split string containing numerical data.
 *
 * @param split_str The result of splitting a string into sections.
 * @param size The number of sections in `split_str`.
 * @param ast Pointer to the AST structure where the processed data will be stored.
 * @param index The index in `split_str` where the numerical data starts.
 *
 * @return int Returns 1 if the numerical data is valid and successfully processed, otherwise returns 0.
 */
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

    /* Init data section */
    while (concat_str[0] != '\0')
    {
        if (concat_str[0] == COMMA_CHAR)
        {
            if (flag_comma == 1)
            {
                strcpy(ast->lineError, "Comma must not be one after another");
                return 0;
            }

            flag_comma = 1, flag_number = 0; /* Set flags */
            concat_str++;                    /* Skip comma */
        }

        else if (concat_str[0] != COMMA_CHAR && concat_str[0] != SPACE_CHAR)
        {
            if (flag_number == 1)
            {
                strcpy(ast->lineError, "Number must be separated by comma");
                return 0;
            }

            flag_number = 1, flag_comma = 0;                                  /* Set flags */
            num = is_number(concat_str, MIN_NUM, MAX_NUM, &result, &end_ptr); /* Check if number is valid */
            switch (result)
            {
            case 0:
                strcpy(ast->lineError, "Invalid number");
                return 0;
            case 1:
                concat_str = end_ptr;        /* Skip number */
                results[data_size_++] = num; /* Add number to data section */
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

/**
 * @brief Checks if a given operand type is valid based on the instruction options.
 *
 * @param operand_type The operand type to be checked.
 * @param inst_options A string representing valid operand types for the instruction.
 *
 * @return int Returns 1 if the operand type is valid according to `inst_options`, otherwise returns 0.
 */
int is_op_valid(int const operand_type, char const *inst_options)
{
    int i;
    for (i = 0; i < strlen(inst_options); i++)
    {
        if (operand_type == inst_options[i] - '0') /* If operand type is valid */
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Determines the type of an operand based on its format and value.
 *
 * @param operand The string representation of the operand.
 * @param ast Pointer to the AST structure where errors will be reported.
 *
 * @return int Returns the operand type if valid, otherwise returns -1 and sets an error in `ast`.
 */
int get_operand_type(char *operand, struct ast *ast)
{
    if (operand[0] == '#' || operand[0] == '*') /* If operand is immediate or register address */
    {
        return ((operand[0] == '#') ? ast_immidiate : ast_register_address);
    }
    else if (is_register(operand)) /* If operand is register */
    {
        return ast_register_direct;
    }
    else if (is_label(operand, ast, NOT_DEFINITION_LABEL)) /* If operand is label */
    {
        return ast_label;
    }
    else
    {
        strcpy(ast->lineError, "Instruction line syntax error"); /* Invalid operand */
        ast->ast_type = ast_error;
        return -1;
    }
}

/**
 * @brief Updates the AST with operand details based on the operand type and value.
 *
 * @param value The string representation of the operand value.
 * @param ast Pointer to the AST structure where the operand will be updated.
 * @param operand_type The type of the operand (immediate, register, register address, or label).
 * @param operand_index The index of the operand in the AST to be updated.
 *
 * @return void This function does not return a value.
 */
void update_ast_operands(char *value, struct ast *ast, int operand_type, int operand_index)
{
    int result = __INT_MAX__; /* Init result */
    int integer_value;
    switch (operand_type)
    {
    case ast_immidiate: /* If operand is immediate */
        ast->ast_options.inst.operands[operand_index].operand_type = ast_immidiate;
        integer_value = is_number(value + 1, MIN_NUM_IMMID, MAX_NUM_IMMID, &result, NULL);
        ast->ast_options.inst.operands[operand_index].operand_option.immed = integer_value;
        break;
    case ast_register_direct: /* If operand is register */
        ast->ast_options.inst.operands[operand_index].operand_type = ast_register_direct;
        integer_value = is_number(value + 1, MIN_NUM, MAX_NUM, &result, NULL);
        ast->ast_options.inst.operands[operand_index].operand_option.reg = integer_value;
        break;
    case ast_register_address: /* If operand is register address */
        ast->ast_options.inst.operands[operand_index].operand_type = ast_register_address;
        integer_value = is_number(value + 2, MIN_NUM, MAX_NUM, &result, NULL);
        ast->ast_options.inst.operands[operand_index].operand_option.reg = integer_value;
        break;
    case ast_label: /* If operand is label */
        ast->ast_options.inst.operands[operand_index].operand_type = ast_label;
        strcpy(ast->ast_options.inst.operands[operand_index].operand_option.label, value);
        break;
    default:
        break;
    }

    switch (result) /* Check if number is valid after calling is_number function (if called) */
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

/**
 * @brief Sets the operands for an instruction with two operands in the AST.
 *
 * @param ast Pointer to the AST structure to be updated with operand information.
 * @param split_result The result of splitting the instruction line, containing the operands.
 *
 * @return void This function does not return a value.
 *
 * @details This function updates the AST with the source and destination operands based on the split result of an instruction line. It performs validation of operand types and updates the AST with the correct operand values. If any operand type is invalid, it sets an error message in the AST.
 */
void set_ast_inst_two_operands(struct ast *ast, struct string_split split_result)
{
    struct inst inst = inst_table[ast->ast_options.inst.inst_type];
    char line_error[MAX_LINE] = {0}, operand_type[MAX_LINE] = {0};
    int source_type = get_operand_type(split_result.string[0], ast);
    int dest_type = get_operand_type(split_result.string[1], ast);

    if (is_op_valid(source_type, inst.source) == 0) /* Check if source operand is valid */
    {
        strcpy(operand_type, "Invalid source operand type in ");
    }

    if (is_op_valid(dest_type, inst.dest) == 0) /* Check if dest operand is valid */
    {
        strcat(operand_type, "Invalid dest operand type in ");
    }

    if (strlen(operand_type) > 0) /* If operand type is invalid */
    {
        strcat(line_error, operand_type);
        strcat(line_error, inst.name);
        strcat(line_error, " instruction");
        strcpy(ast->lineError, line_error);
        ast->ast_type = ast_error;
        return;
    }

    update_ast_operands(split_result.string[0], ast, source_type, 0); /* Update source operand */
    update_ast_operands(split_result.string[1], ast, dest_type, 1);   /* Update destination operand*/
}

/**
 * @brief Sets the operand for an instruction with one operand in the AST.
 *
 * @param ast Pointer to the AST structure to be updated with operand information.
 * @param split_result The result of splitting the instruction line, containing the operand.
 *
 * @return void This function does not return a value.
 *
 * @details This function updates the AST with the destination operand based on the split result of an instruction line. It validates the operand type and updates the AST with the correct operand value. If the operand type is invalid, it sets an error message in the AST.
 */
void set_ast_inst_one_operands(struct ast *ast, struct string_split split_result)
{
    struct inst inst = inst_table[ast->ast_options.inst.inst_type];
    char line_error[MAX_LINE] = {0};
    int dest_type = get_operand_type(split_result.string[0], ast); /* Get operand type */

    if (is_op_valid(dest_type, inst.dest) == 0) /* Check if dest operand is valid */
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

/**
 * @brief Concatenates a subset of strings from the string_split structure into a single string.
 *
 * @param split_result The result of splitting a string into multiple parts.
 * @param index The starting index of the subset to be concatenated.
 * @param size The number of strings to concatenate from the starting index.
 *
 * @return char* A pointer to the newly allocated and concatenated string.
 */
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

/**
 * @brief Checks if there is a comma following the specified operand in a given string.
 *
 * @param string The string in which to search for the operand and comma.
 * @param first_operand The operand to check for a following comma.
 * @param second_operand The second operand to check for a preceding comma.
 *
 * @return int Returns 1 if a comma is found after the specified operand, 0 if no comma is found, and -1 if the operand is not found in the string.
 */
int has_comma_between_operands(const char *string, const char *first_operand, const char *second_operand)
{
    const char *ptr = strstr(string, first_operand); /* Find first_operand in string */
    const char *ptr2;
    int index, end, i;

    if (ptr != NULL)
    {
        index = ptr - string; /* Index of first_operand in string */
    }
    else
    {
        return -1;
    }

    ptr2 = strstr(string + strlen(first_operand), second_operand); /* Find second_operand in string */

    if (ptr2 != NULL)
    {
        end = ptr2 - string; /* Index of second_operand in string */
    }
    else
    {
        return -1;
    }

    for (i = index; i < end; i++) /* Check if comma is after first_operand */
    {
        if (string[i] == COMMA_CHAR)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Parses and processes operands from a string split result and updates the AST accordingly.
 *
 * @param operands The split result containing operands.
 * @param index The starting index of the operands in the split result.
 * @param ast The AST structure to be updated with parsed operand information.
 */
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

        if ((index + 1) < operands.size && !has_comma_between_operands(original_concat_string, temp_split_str.string[0], temp_split_str.string[1]))
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

/**
 * @brief Fills the AST with directive information based on the split result.
 *
 * @param ast The AST structure to be updated.
 * @param split_result The split result containing the directive and associated data.
 * @param index The index in the split result where the directive starts.
 */
void fill_directive_ast(struct ast *ast, struct string_split split_result, int index)
{
    ast->ast_type = ast_dir;

    if (strcmp(split_result.string[index], DIRECTIVE_DATA) == 0) /* If directive is .data */
    {
        ast->ast_options.dir.dir_type = ast_data;
        if (!validate_numbers(split_result, split_result.size, ast, ++index))
        {
            ast->ast_type = ast_error;
        }
    }
    else if (strcmp(split_result.string[index], DIRECTIVE_STRING) == 0) /* If directive is .string */
    {
        ast->ast_options.dir.dir_type = ast_string;
        if (!fill_string(split_result, index + 1, ast))
        {
            ast->ast_type = ast_error;
        }
    }
    else if (strcmp(split_result.string[index], DIRECTIVE_ENTRY) == 0 || strcmp(split_result.string[index], DIRECTIVE_EXTERN) == 0) /* If directive is .entry or .extern */
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
        strcpy(ast->lineError, "Invalid directive"); /* Invalid directive */
        ast->ast_type = ast_error;
    }
}

/**
 * @brief Parses a line of text and populates an `ast` structure based on its content.
 *
 * This function analyzes a line of text, determines whether it represents a comment,
 * a label, a directive, or an instruction, and fills the provided `ast` structure
 * with the appropriate data. It handles various types of lines and validates them
 * according to specific rules.
 *
 * @param line The line of text to be parsed. This should be a null-terminated string.
 * @param macro_table A pointer to a `MacroContext` structure containing macro definitions.
 *                    This can be `NULL` if macros are not used.
 *
 * @return A `struct ast` containing the parsed information.
 */
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
