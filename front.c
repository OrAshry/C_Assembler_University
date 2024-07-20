#include "front.h"
#include <ctype.h>
#include "helpingFunction.c"

static int is_number(char *str, int min_num, int max_num, int *result)
{
    /* using strtol */
}

static int validate_numbers(struct string_split split_str, int index, int size, struct ast *ast)
{
    int i;
    int result;
    char * concat_str = (char *)allocateMemory(MAX_LINE, sizeof(char), CALLOC_ID);
    int results[size];

    /* Concat substring to single string */
    for(i = index; i < size; i++){
        strcat(concat_str, split_str.string[i]);
        if(i != size - 1){ /* Adding space between substrings */
            strcat(concat_str, SPACE);
        }
    }

    /* If first or last char in .data is comma , */
    if (concat_str[0] == COMMA_CHAR || concat_str[size] == COMMA_CHAR)
    {
        strcpy(ast->lineError, "Comma must not be at the start or end of the line");
        return 0;
    }

    for (i = 0; i < size; i++)
    {
        if (concat_str[i] == COMMA_CHAR && (i + 1) < size && concat_str[i + 1] == COMMA_CHAR)
        {
            strcpy(ast->lineError, "Comma must not be one after another");
            return 0;
        }

        if (concat_str[i] != COMMA_CHAR)
        {
            is_number(concat_str, MIN_NUM, MAX_NUM, &result);
            switch (result)
            {
            case 0:
                strcpy(ast->lineError, "Invalid number");
                return 0;
            case 1:
                results[i] = result;
                break;
            case 2:
                strcpy(ast->lineError, "Number is too big");
                return 0;
            case 3:
                strcpy(ast->lineError, "Number is too small");
                return 0;
            }
        }
    }

    ast->ast_options.dir.dir_options.data = results;
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
    /* check if the string is a register */
}

static int is_instruction(char *str)
{
    return 0;
    /* check if the string is an instruction */
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
}

struct string_split split_string(char *str)
{
    int strings_count = 0;
    char *s;
    struct string_split split_result = {0};
    while (isspace(*str))
        str++; /* Skiping leading whitespcaes */

    if (*str == '\0')
    { /* If string is empty after remove whitespaces */
        return split_result;
    }

    while (str && *str != '\0')
    {
        /* Store current string in list */
        split_result.string[strings_count++] = str;

        /* Move to next string */
        str = strpbrk(str, SPACES);

        if (str)
        {
            *str = '\0'; /* Null termiante */
            str++;

            /* Skip additional whitespaces */
            while (isspace(*str))
            {
                str++;
            }
        }
    }

    split_result.size = strings_count; /* Update strings counter in list */
    return split_result;               /* Return string_split structure */
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
    char line[100];
    strcpy(line, "-3 2,1,1");
    struct string_split split_result = split_string(line);
    struct ast ast2 = {0};
    validate_numbers(split_result, 0, 2, &ast2);
    int i;
    struct ast ast = get_ast_from_line(line);
    return 0;
}