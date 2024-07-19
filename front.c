#include "front.h"
#include <ctype.h>

static int is_number(char *str, int min_num, int max_num, int *result)
{
    /* using strtol */
}

static int is_label(char *str)
{
    /* check if the string is a label */
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

    /* Directive line case (data, string, entry, extern) */
    /* OR Instruction line case with label at start */
    if (is_label(split_result.string[index]))
    {
        ast.labelName = split_result.string[index++]; /* Init label name */

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

    /* If current line is directive line with .*/
    /* Without label at start of line */
    if (split_result.string[index][0] == DIRECTIVE_CHAR)
    {
        fill_directive_ast(&ast, split_result, index);
        return ast;
    }

    /* If current line is instruction line without a label at start */
    parse_operands(split_result.string[index], &ast);
    return ast;
}

int main()
{
    char line[100];
    strcpy(line, "     ;fiksdosd");
    int i;
    struct ast ast = get_ast_from_line(line);
    return 0;
}