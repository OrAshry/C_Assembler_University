#include "SecondPass.h"

int secondPass(char * file_name, FILE * file) {
    
    /* Declarations */
    int error_flag = 0;
    int am_line_counter = 1; /* After macro line counter */
    char line[MAX_LINE_LENGTH]; /* The line muber of the source file after macro */
    struct ast answer_line = {0};
    int L; /* Words counter*/
    int i;

    while(fgets(line, MAX_LINE_LENGTH, file)) {
        answer_line = get_ast_from_line(line);
        L = 0;

        /* Calculate words and code the code into code_image */
        if((answer_line.ast_type = ast_inst)) {
            
            /* Calculate words */
            for (i = 0; i < 2; i++) {
                if (answer_line.ast_options.inst.operands[i].operand_type != ast_none) {
                    L++;
                }
            }
            (machine_code_ptr -> IC) += L;

            /* Code the first word inside of code_image */
            machine_code_ptr -> code_image[machine_code_ptr -> IC] = answer_line.ast_options.inst.operands[0].operand_type << 7; /* Source operand */
            machine_code_ptr -> code_image[machine_code_ptr -> IC] |= answer_line.ast_options.inst.operands[1].operand_type << 3; /* Destenation operand */
            machine_code_ptr -> code_image[machine_code_ptr -> IC] |= answer_line.ast_options.inst.inst_type << 11; /* Opecode */
            (machine_code_ptr -> IC)++;
        }

        am_line_counter++;
    }
    
    return error_flag;
}
