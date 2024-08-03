
#include "constants.h"

#define ERROR_LINE 200
#define SPACES " \t\v\f"
#define COMMA ","
#define COMMENT_CHAR ';'
#define DIRECTIVE_CHAR '.'
#define LABEL_CHAR ':'
#define MAX_LABEL_SIZE 31
#define COMMA_CHAR ','
#define NULL_BYTE '\0'
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

struct string_split{
    char * string[MAX_LINE];
    int size;
};

struct inst {
    char * name;
    int opcode;
    const char *source;
    const char *dest;
};

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
    {"stop", 15, "", ""}
};

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
int validate_numbers(struct string_split split_str, int size, struct ast *ast, int index);
int is_op_valid(int const operand_type, char const * inst_options);
void parse_operand(char * operand, int operand_type ,struct ast * ast, int operand_index);
void parse_operands(struct string_split operands, int index, struct ast * ast);
struct string_split split_string(char * str, const char * delimiter);
int fill_string(struct string_split split_result, int index, struct ast *ast);
void fill_directive_ast(struct ast * ast, struct string_split split_result, int index);
int validate_numbers(struct string_split const split_str, int const size, struct ast *ast, int const index);