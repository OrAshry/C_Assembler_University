#include "SecondPass.h"

int secondPass(char * file_name, FILE * file) {
    
    /* Declarations */
    int error_flag = 0;
    char line[MAX_LINE_LENGTH]; /* The line muber of the source file after macro */
    struct ast answer_line = {0};
    int L; /* Words counter*/
    int i;

    while(fgets(line, MAX_LINE_LENGTH, file)) {
        answer_line = get_ast_from_line(line);
        L =0;

        /* Calculate words and code data into data_image */
        if((answer_line.ast_type == ast_dir) && ((answer_line.ast_options.dir.dir_type == ast_data) || (answer_line.ast_options.dir.dir_type == ast_string))) {
            memcpy(&machine_code_ptr -> data_image[machine_code_ptr -> DC], answer_line.ast_options.dir.dir_options.data, answer_line.ast_options.dir.dir_options.data_size);
            L = answer_line.ast_options.dir.dir_options.data_size;
            (machine_code_ptr -> DC) += L;

        }

        /* Calculate words and code the code into code_image */
        if((answer_line.ast_type = ast_inst)) {
            
            /* Calculate words */
            for (i = 0; i < 2; i++) {
                if (answer_line.ast_options.inst.operands[i].operand_type != ast_none) {
                    L++;
                }
            }
            (machine_code_ptr -> IC) += L;

            /* Code the code into code_image */

        }
    }

    return error_flag;
}
