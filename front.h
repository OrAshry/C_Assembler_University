
#define ERROR_LINE 200

struct ast{
    char lineError[ERROR_LINE];
    const char * labelName; 
    enum{
        ast_inst,
        ast_dir,
        ast_error,
        ast_note,
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
                    ast_register
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