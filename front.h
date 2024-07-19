
#include "constants.h"

#define ERROR_LINE 200
#define SPACES " \t\v\f"
#define COMMENT_CHAR ';'
#define DIRECTIVE_CHAR '.'

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

struct inst inst_table[16] = {
    {"mov", 0, "0123", "123"},
    {"cmp", 1, "0123", "0123"},
    {"add", 2, "0123", "123"},
    {"sub", 2, "0123", "123"},
    {"lea", 4, "1", "123"},
    {"clr", 5, "", "123"},
    {"not", 5, "", "123"},
    {"inc", 5, "", "123"},
    {"dec", 5, "", "123"},
    {"jmp", 9, "", "12"},
    {"bne", 9, "", "12"},
    {"red", 12, "", "123"},
    {"prn", 13, "", "0123"},
    {"jsr", 14, "", "12"},
    {"rts", 15, "", ""},
    {"stop", 15, "", ""}
};

struct ast{
    char lineError[ERROR_LINE];
    const char * labelName; 
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
            union{
                char * label;
                int * data;
                int data_size;
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
                    ast_none,
                    ast_immidiate,
                    ast_label,      
                    ast_register_direct,
                    ast_register_address
                }operand_type;
                union{
                    int immed;
                    char * label;
                    int reg;
                }operand_option;
            }operands[2];
        }inst;

    }ast_options;
};

struct ast get_ast_from_line(char * line);
static int is_number(char * str, int min_num, int max_num, int * result);
static int is_label(char * str);
static void parse_operand(char * operand, int operand_type ,struct ast * ast, struct inst * inst);
static void parse_operands(char * operands, struct ast * ast);
struct string_split split_string(char * str);