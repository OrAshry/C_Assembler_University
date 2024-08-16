#ifndef LINEPARSER_H
#define LINEPARSER_H

#include "stringSplit.h"
#include "Constants.h"
#include <ctype.h>
#include <string.h>
#include "helpingFunction.h"
#include <stdlib.h>

#define ERROR_LINE 200
#define SPACES " \t\v\f"
#define COMMA ","
#define COMMENT_CHAR ';'
#define DIRECTIVE_CHAR '.'
#define LABEL_CHAR ':'
#define MAX_LABEL_SIZE 31
#define COMMA_CHAR ','
#define MIN_NUM -16384
#define MAX_NUM 16383
#define SPACE_CHAR ' '
#define SPACE " "
#define INST_SIZE 16
#define DECIMAL_BASE 10
#define RESULT_ARR_SIZE 250
#define DIRECTIVE_DATA ".data"
#define DIRECTIVE_STRING ".string"
#define DIRECTIVE_EXTERN ".extern"
#define DIRECTIVE_ENTRY ".entry"
#define STRING_CHAR '"'
#define REGISTER_MIN 1
#define REGISTER_MAX 7
#define REGISTER_CHAR 'r'
#define DEFINITION_LABEL 1
#define NOT_DEFINITION_LABEL 0

struct inst {
    char * name;
    int opcode;
    const char *source;
    const char *dest;
};

extern struct inst inst_table[INST_SIZE];

struct ast{
    char lineError[ERROR_LINE];
    char labelName[MAX_LABEL_SIZE]; 
    enum{
        ast_inst,
        ast_dir,
        ast_error,
        ast_comment,
        ast_empty
    }ast_type;
    union{

        struct{
            enum{
                ast_extern,
                ast_entry,
                ast_string,
                ast_data
            }dir_type;
            struct{
                char label[MAX_LABEL_SIZE];
                int data_size;
                int data[RESULT_ARR_SIZE];
            }dir_options;
        }dir;

        struct{
            enum{
                ast_move, ast_cmp, ast_add,
                ast_sub, ast_lea, ast_clr,
                ast_not, ast_inc, ast_dec,
                ast_jmp, ast_bne, ast_red,
                ast_prn, ast_jsr, ast_rts,
                ast_stop
            }inst_type;
            struct{
                enum{
                    ast_immidiate,
                    ast_label,      
                    ast_register_address,
                    ast_register_direct,
                    ast_none
                }operand_type;
                union{
                    int immed;
                    char label[MAX_LABEL_SIZE];
                    int reg;
                }operand_option;
            }operands[2];
        }inst;

    }ast_options;
};

struct ast get_ast_from_line(char * line);
int is_number(char * str, int min_num, int max_num, int * result, char ** end_ptr);
int is_op_valid(int const operand_type, char const * inst_options);
void parse_operands(struct string_split operands, int index, struct ast * ast);
int fill_string(struct string_split split_result, int index, struct ast *ast);
void fill_directive_ast(struct ast * ast, struct string_split split_result, int index);
int validate_numbers(struct string_split const split_str, int const size, struct ast *ast, int const index);
void set_ast_inst_two_operands(struct ast * ast, struct string_split split_result, int index);
void set_ast_inst_one_operands(struct ast * ast, struct string_split split_result, int index);
int get_operand_type(char *operand, struct ast *ast);
void update_ast_operands(char *value, struct ast * ast, int operand_type, int operand_index);

#endif
