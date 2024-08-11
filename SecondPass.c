#include "SecondPass.h"

int secondPass(char * file_name, FILE * file) {
    
    /* Declarations */
    int error_flag = 0;
    int am_line_counter = 1; /* After macro line counter */
    int L; /* Words counter */
    int i;
    int two_op_reg; /* Flag that indicates if the there are 2 operands of type register*/
    int A = 2;
    int R = 1;
    int E = 0;
    char line[MAX_LINE_LENGTH]; /* The line muber of the source file after macro */
    struct ast answer_line = {0};
    machine_code_ptr -> IC = 0; /* Restart inst counter */

    while(fgets(line, MAX_LINE_LENGTH, file)) {
        answer_line = get_ast_from_line(line);
        two_op_reg = 0;
        L = 1;

        /* Check for the use of label that wasnt defined */


        /* Calculate words and code the code into code_image */
        if((answer_line.ast_type = ast_inst)) {
            
            /* Calculate words */
            for (i = 0; i < 2; i++) {
                if (answer_line.ast_options.inst.operands[i].operand_type != ast_none) {
                    L++;
                }
                /* if there are 2 operands both registers */
                if(L == 3) {
                    if(((answer_line.ast_options.inst.operands[0].operand_type == ast_register_direct) || (answer_line.ast_options.inst.operands[0].operand_type == ast_register_address)) &&
                        ((answer_line.ast_options.inst.operands[1].operand_type == ast_register_direct) || (answer_line.ast_options.inst.operands[1].operand_type == ast_register_address))) {
                        two_op_reg = 1;    
                        L--;
                    }
                }
            }

            /* Check that the progrem has not reached maximum memmory size */
            if((strlen(machine_code_ptr -> data_image)) + (strlen(machine_code_ptr -> code_image)) + L > MAX_MEM_SIZE) {
                error_flag = 1;
                printf("Error: the program has reached maximum memmory size allowed.\n ");
                return error_flag;
            }
                
            /* Initialize IC if needed */
            if(machine_code_ptr -> IC == 0) {
                machine_code_ptr -> IC = 100;
            }

            /* Code the first word inside of code_image */
            machine_code_ptr -> code_image[machine_code_ptr -> IC] = 1 << A; /* A,R,E */
            /* Destination operand and source operand*/
            if(L == 3) {
                machine_code_ptr->code_image[machine_code_ptr->IC] |= (answer_line.ast_options.inst.operands[1].operand_type << 3); /* Destenation operand */
                machine_code_ptr->code_image[machine_code_ptr->IC] |= (answer_line.ast_options.inst.operands[0].operand_type << 7); /* Source opernand*/
            }
            /* Only destination operand or 2 registers operands*/
            else if(L == 2) {
                /* 2 operands both registers */
                if(two_op_reg) {
                    machine_code_ptr->code_image[machine_code_ptr->IC] |= (answer_line.ast_options.inst.operands[1].operand_type << 3); /* Destenation operand */
                    machine_code_ptr->code_image[machine_code_ptr->IC] |= (answer_line.ast_options.inst.operands[0].operand_type << 7); /* Source opernand*/
                }
                /* Only destination */
                else {
                    machine_code_ptr->code_image[machine_code_ptr->IC] |= (answer_line.ast_options.inst.operands[1].operand_type << 3); /* Destenation operand */
                }
            }
            machine_code_ptr -> code_image[machine_code_ptr -> IC] |= answer_line.ast_options.inst.inst_type << 11; /* Opecode */
            (machine_code_ptr -> IC)++;

            /* Code the second and third word*/
            if(L == 3) { /* If there are 2 operands and at least one of them is not registar */
                codeWords(L, answer_line, A, R, E, &error_flag, file_name, am_line_counter);
            }            
            else if(L == 2) { /* If there is only one operand or two register operands */
                /* Two register operands*/
                if(two_op_reg) {
                    machine_code_ptr -> code_image[machine_code_ptr -> IC] = 1 << A;
                    machine_code_ptr -> code_image[machine_code_ptr -> IC] |= answer_line.ast_options.inst.operands[1].operand_option.reg << 3; /* Destination reg num*/
                    machine_code_ptr -> code_image[machine_code_ptr -> IC] |= answer_line.ast_options.inst.operands[1].operand_option.reg << 6; /* Source reg num*/
                }
                /* Only destination operand*/
                else {
                    codeWords(L, answer_line, A, R, E, &error_flag, file_name, am_line_counter);
                }
            }
        }

        am_line_counter++;
    }
    
    return error_flag;
}

/* This function will code the second and third words */
void codeWords(int num_of_words, struct ast a, int absolute_word, int relocatable_word, int external_word, int *flag, const char *name_of_file, int current_am_line) {
    int i;

    for(i = 0; i < num_of_words - 1; i++, (machine_code_ptr -> IC)++) {
        if(a.ast_options.inst.operands[i].operand_type == ast_immidiate) {
            machine_code_ptr -> code_image[machine_code_ptr -> IC] = 1 << absolute_word; /* A,R,E */
            machine_code_ptr -> code_image[machine_code_ptr -> IC] |= a.ast_options.inst.operands[i].operand_option.immed << 3; /* Operand */
        }
        else if(a.ast_options.inst.operands[i].operand_type == ast_label) {
            found = symbol_search(head_ptr, a.ast_options.inst.operands[i].operand_option.label);
            if(!found) {
                printf("Error: In file %s at line %d the symbol %s has been never defined.\n", name_of_file, current_am_line, a.ast_options.inst.operands[i].operand_option.label);
                *flag = 1;
                return;
            }
            else if(found -> symbol_type == extern_symbol) {
                machine_code_ptr -> code_image[machine_code_ptr -> IC] = 1 << external_word; /* A,R,E */
            }
            else {
                machine_code_ptr -> code_image[machine_code_ptr -> IC] = 1 << relocatable_word; /* A,R,E */
            } 
            machine_code_ptr -> code_image[machine_code_ptr -> IC] |= found -> symbol_address << 3; /*Address of the label*/
            }
        else if((a.ast_options.inst.operands[i].operand_type == ast_register_address) || (a.ast_options.inst.operands[i].operand_type == ast_register_direct)) {
            machine_code_ptr -> code_image[machine_code_ptr -> IC] = 1 << absolute_word; /* A,R,E */
            if(i == 0) { /* First operand (source)*/
                machine_code_ptr -> code_image[machine_code_ptr -> IC] |= a.ast_options.inst.operands[i].operand_option.reg << 6; /* Register number */
            }
            else if(i == 1) /* Second operand (destination)*/
            {
                machine_code_ptr -> code_image[machine_code_ptr -> IC] |= a.ast_options.inst.operands[i].operand_option.reg << 3; /* Register number */
            }
        }
            
    }
}
