#ifndef LINEPARSER_H
#define LINEPARSER_H

#include "macroContext.h"
#include "MacroProcessing.h"
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
#define MIN_NUM_IMMID -2048
#define MAX_NUM_IMMID 2047
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
#define REGISTER_MIN 0
#define REGISTER_MAX 7
#define REGISTER_CHAR 'r'
#define DEFINITION_LABEL 1
#define NOT_DEFINITION_LABEL 0

/**
 * @brief Structure to represent an instruction.
 */
struct inst
{
    char *name;         /**< Instruction name */
    int opcode;         /**< Opcode of the instruction */
    const char *source; /**< Valid source operand types for this instruction */
    const char *dest;   /**< Valid destination operand types for this instruction */
};

extern struct inst inst_table[INST_SIZE];

/**
 * @brief Structure to represent an abstract syntax tree (AST) node.
 */
struct ast
{
    char lineError[ERROR_LINE];     /**< Error message for the line */
    char labelName[MAX_LABEL_SIZE]; /**< Label name, if applicable */
    enum
    {
        ast_inst,    /**< Instruction line */
        ast_dir,     /**< Directive line */
        ast_error,   /**< Error line */
        ast_comment, /**< Comment line */
        ast_empty    /**< Empty line */
    } ast_type;      /**< Type of AST node */
    union
    {
        struct
        {
            enum
            {
                ast_extern, /**< Extern directive */
                ast_entry,  /**< Entry directive */
                ast_string, /**< String directive */
                ast_data    /**< Data directive */
            } dir_type;     /**< Type of directive */
            struct
            {
                char label[MAX_LABEL_SIZE]; /**< Label for the directive */
                int data_size;              /**< Size of data */
                int data[RESULT_ARR_SIZE];  /**< Data array */
            } dir_options;                  /**< Options for the directive */
        } dir;
        struct
        {
            enum
            {
                ast_move,
                ast_cmp,
                ast_add,
                ast_sub,
                ast_lea,
                ast_clr,
                ast_not,
                ast_inc,
                ast_dec,
                ast_jmp,
                ast_bne,
                ast_red,
                ast_prn,
                ast_jsr,
                ast_rts,
                ast_stop
            } inst_type; /**< Type of instruction */
            struct
            {
                enum
                {
                    ast_immidiate,        /**< Immediate operand */
                    ast_label,            /**< Label operand */
                    ast_register_address, /**< Register address operand */
                    ast_register_direct,  /**< Register direct operand */
                    ast_none              /**< No operand */
                } operand_type;           /**< Type of operand */
                union
                {
                    int immed;                  /**< Immediate value */
                    char label[MAX_LABEL_SIZE]; /**< Label value */
                    int reg;                    /**< Register value */
                } operand_option;               /**< Options for the operand */
            } operands[2];                      /**< Operands for the instruction */
        } inst;
    } ast_options; /**< Options based on AST type */
};

/* Prototype Functions */
struct ast get_ast_from_line(char *line, struct MacroContext *macro_table);
int is_number(char *str, int min_num, int max_num, int *result, char **end_ptr);
int is_op_valid(int const operand_type, char const *inst_options);
void parse_operands(struct string_split operands, int index, struct ast *ast);
int fill_string(struct string_split split_result, int index, struct ast *ast);
void fill_directive_ast(struct ast *ast, struct string_split split_result, int index);
int validate_numbers(struct string_split const split_str, int const size, struct ast *ast, int const index);
void set_ast_inst_two_operands(struct ast *ast, struct string_split split_result);
void set_ast_inst_one_operands(struct ast *ast, struct string_split split_result);
int get_operand_type(char *operand, struct ast *ast);
void update_ast_operands(char *value, struct ast *ast, int operand_type, int operand_index);
int is_defined_macro(char *label, struct MacroContext *macro_table);
char *concat_string_split(struct string_split split_result, int const index, int const size);
int has_comma_between_operands(const char *string, const char *first_operand, const char *second_operand);

#endif
