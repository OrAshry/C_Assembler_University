#include "secondPass.h"

/**
 * @brief Performs the second pass over the assembly file, processing instructions and tracking external symbol usage.
 *
 * This function processes the given assembly file in the second pass. It calculates the number of words needed
 * for instructions, codes the instructions into the code image, and tracks the usage of external symbols.
 * If an error occurs (such as exceeding the maximum memory size or using undefined symbols), it will set
 * an error flag and return NULL. The function also updates the `extern_usage_head_ptr` with external symbol usage.
 *
 * @param file_name The name of the assembly file being processed.
 * @param file A pointer to the file stream of the assembly file.
 * @param extern_usage_head_ptr A pointer to the head of the external symbols usage list.
 * @param err_flag A pointer to an integer that will be set to 1 if any errors are encountered.
 * @return A pointer to the updated external symbols usage list, or NULL if an error occurs.
 */
extern_addresses_ptr secondPass(char *file_name, FILE *file, extern_addresses_ptr extern_usage_head_ptr, int *err_flag)
{
    /* Declarations */
    int error_flag = 0;
    int skip_to_next_line; /* Indicate that the current line should be skipped if an error occurs */
    int am_line_counter = 1; /* After macro line counter */
    int L; /* Words counter */
    int i;
    int two_op_reg; /* Flag that indicates if the there are 2 operands of type register*/
    char line[MAX_LINE_LENGTH] = {0}; /* The line muber of the source file after macro */
    extern_addresses_ptr extern_ptr = extern_usage_head_ptr;
    struct ast answer_line = {0};
    machine_code_ptr->IC = 0; /* Restart inst counter */
    
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        answer_line = get_ast_from_line(line, NULL);
        two_op_reg = 0;
        L = 1;
        skip_to_next_line = 0;
        
        /* Calculate words and code the code into code_image */
        if (answer_line.ast_type == ast_inst)
        {
            /* Calculate words */
            for (i = 0; i < 2; i++)
            {
                if (answer_line.ast_options.inst.operands[i].operand_type != ast_none)
                {
                    L++;
                }
                /* if there are 2 operands both registers */
                if (L == 3)
                {
                    if (((answer_line.ast_options.inst.operands[0].operand_type == ast_register_direct) || (answer_line.ast_options.inst.operands[0].operand_type == ast_register_address)) &&
                        ((answer_line.ast_options.inst.operands[1].operand_type == ast_register_direct) || (answer_line.ast_options.inst.operands[1].operand_type == ast_register_address)))
                    {
                        two_op_reg = 1;
                        L--;
                    }
                }
            }

            /* Check that the program has not reached maximum memmory size */
            if (((machine_code_ptr->DC) + (machine_code_ptr->IC) + L - 100) > MAX_MEM_SIZE)
            {
                error_flag = 1;
                printf("Error: the program has reached maximum memmory size allowed.\n ");
                *err_flag = error_flag;
                return NULL;
            }

            /* Initialize IC if needed */
            if (machine_code_ptr->IC == 0)
            {
                machine_code_ptr->IC = 100;
            }

            /* Initialzie the extern_usage struct and checks if there is a label that been used without a declaration*/
            if((L == 3) || ((L == 2) && (two_op_reg == 0))) /* Two operands*/
            {
                for(i = 0; i < L - 1; i++)
                {
                    if(answer_line.ast_options.inst.operands[i].operand_type == ast_label) 
                    {
                        found = symbol_search(head_ptr, answer_line.ast_options.inst.operands[i].operand_option.label);
                        if(found) 
                        {
                            if (found->symbol_type == extern_symbol) 
                            {
                                extern_ptr = find_extern(extern_ptr, answer_line.ast_options.inst.operands[i].operand_option.label);
                                if (extern_ptr)
                                {
                                    /* Assign addresses */
                                    if (i == 0) 
                                    {
                                        extern_ptr->used_addresses[extern_ptr->used_counter] = (machine_code_ptr->IC) + 1;
                                    }
                                    else 
                                    {
                                        extern_ptr->used_addresses[extern_ptr->used_counter] = (machine_code_ptr->IC) + 2;
                                    }
                                    /* Increasing the used counter */
                                    (extern_ptr->used_counter)++;
                                }

                                /* Adding a new extern symbol for a symbol that has been used for the first time */
                                else 
                                {
                                    if(i == 0)
                                    {
                                        add_symbol_to_extern_usage(answer_line.ast_options.inst.operands[i].operand_option.label, (machine_code_ptr->IC) + 1, &extern_usage_head_ptr);
                                    }
                                    else 
                                    {
                                        add_symbol_to_extern_usage(answer_line.ast_options.inst.operands[i].operand_option.label, (machine_code_ptr->IC) + 2, &extern_usage_head_ptr);

                                    }
                                }
                            }

                        }
                        else /* there is a usage of a label and it is not defiend */
                        {
                            printf("Error: In file %s at line %d the symbol %s has been never defined.\n", file_name, am_line_counter, answer_line.ast_options.inst.operands[i].operand_option.label);
                            error_flag = 1;
                            skip_to_next_line = 1; 
                        }
                    }
                }
            }

            if(skip_to_next_line) {
                continue;
            }

            /* Code the first word inside of code_image */
            machine_code_ptr->code_image[machine_code_ptr->IC] = 1 << A; /* A,R,E */

            /* Destination operand and source operand*/
            if (L == 3)
            {
                machine_code_ptr->code_image[machine_code_ptr->IC] |= (1 << (3 + answer_line.ast_options.inst.operands[1].operand_type)); /* Destenation operand */
                machine_code_ptr->code_image[machine_code_ptr->IC] |= (1 << (7 + answer_line.ast_options.inst.operands[0].operand_type)); /* Source opernand*/
            }
            /* Only destination operand or 2 registers operands*/
            else if (L == 2)
            {
                /* 2 operands both registers */
                if (two_op_reg)
                {
                    machine_code_ptr->code_image[machine_code_ptr->IC] |= (1 << (3 + answer_line.ast_options.inst.operands[1].operand_type)); /* Destenation operand */
                    machine_code_ptr->code_image[machine_code_ptr->IC] |= (1 << (7 + answer_line.ast_options.inst.operands[0].operand_type)); /* Source opernand*/
                }
                /* Only destination */
                else
                {
                    machine_code_ptr->code_image[machine_code_ptr->IC] |= (1 << (3 + answer_line.ast_options.inst.operands[0].operand_type)); /* Destenation operand */
                }
            }

            /* Opcode */
            machine_code_ptr->code_image[machine_code_ptr->IC] |= answer_line.ast_options.inst.inst_type << 11;
            (machine_code_ptr->IC)++;

            /* Code the second and third word*/
            if (L == 3)
            {
                /* If there are 2 operands and at least one of them is not register */
                codeWords(L, answer_line, &error_flag, file_name, am_line_counter);
            }
            else if (L == 2)
            { /* If there is only one operand or two register operands */
                /* Two register operands*/
                if (two_op_reg)
                {
                    machine_code_ptr->code_image[machine_code_ptr->IC] = 1 << A;
                    machine_code_ptr->code_image[machine_code_ptr->IC] |= answer_line.ast_options.inst.operands[1].operand_option.reg << 3; /* Destination reg num*/
                    machine_code_ptr->code_image[machine_code_ptr->IC] |= answer_line.ast_options.inst.operands[0].operand_option.reg << 6; /* Source reg num*/
                    (machine_code_ptr->IC)++;                                                                                               /* Move to next IC */
                }
                /* Only destination operand*/
                else
                {
                    codeWords(L, answer_line, &error_flag, file_name, am_line_counter);
                }
            }
        }

        am_line_counter++;
    }
    *err_flag = error_flag;
    return extern_usage_head_ptr;
}


/**
 * @brief Encodes the second and third words of an instruction into the code image.
 *
 * This function encodes the operands of an instruction into the code image, based on the addressing mode
 * (immediate, label, or register). The function adjusts the bit positions based on the type of operand and
 * the number of words being encoded. If an error occurs during encoding, the error flag is set.
 *
 * @param num_of_words The number of words to encode (usually 2 or 3).
 * @param a The abstract syntax tree (AST) node representing the instruction to encode.
 * @param flag A pointer to an integer that will be set to 1 if any errors are encountered.
 * @param name_of_file The name of the assembly file being processed.
 * @param current_am_line The current line number in the assembly file after macro expansion.
 */
void codeWords(int num_of_words, struct ast a, int *flag, const char *name_of_file, int current_am_line)
{
    int i;
    int val;

    for (i = 0; i < num_of_words - 1; i++, (machine_code_ptr->IC)++) 
    {
        
        /* Checking how much to move the bits */
        if((i == 0 && (num_of_words == 2)) || (i == 1) || (a.ast_options.inst.operands[i].operand_type == ast_immidiate || (a.ast_options.inst.operands[i].operand_type == ast_label))) 
        {
            val = 3;
        }
        else 
        {
            val = 6;
        }
        /* If the addressing method is immidiate */
        if (a.ast_options.inst.operands[i].operand_type == ast_immidiate)
        {
            machine_code_ptr->code_image[machine_code_ptr->IC] = 1 << A; /* A,R,E */
            machine_code_ptr->code_image[machine_code_ptr->IC] |= a.ast_options.inst.operands[i].operand_option.immed << val; /* Operand */
        }

        /* If the addressing method is label*/
        else if (a.ast_options.inst.operands[i].operand_type == ast_label)
        {
            found = symbol_search(head_ptr, a.ast_options.inst.operands[i].operand_option.label);
            if (found->symbol_type == extern_symbol)
            {
                machine_code_ptr->code_image[machine_code_ptr->IC] = 1 << E; /* A,R,E */
            }
            else
            {
                machine_code_ptr -> code_image[machine_code_ptr->IC] = 1 << R; /* A,R,E */
            }
            machine_code_ptr -> code_image[machine_code_ptr->IC] |= found -> symbol_address << val; /*Address of the label*/
        }

        /* If the addressing method is register_address or register_direct*/
        else if ((a.ast_options.inst.operands[i].operand_type == ast_register_address) || (a.ast_options.inst.operands[i].operand_type == ast_register_direct)) 
        {
            machine_code_ptr->code_image[machine_code_ptr->IC] = 1 << A; /* A,R,E */
            machine_code_ptr->code_image[machine_code_ptr->IC] |= a.ast_options.inst.operands[i].operand_option.reg << val; /* Register number */
        }
    }
}

/**
 * @brief Frees the memory allocated for the external symbols usage list.
 *
 * This function iterates through the external symbols usage list and frees the memory allocated
 * for each node. It also resets the symbol names and usage addresses within each node to ensure
 * proper cleanup.
 *
 * @param head A pointer to the head of the external symbols usage list.
 */
void free_extern_table(extern_addresses_ptr head){
    int i;
    extern_addresses_ptr current = head;
    extern_addresses_ptr next;

    while (current != NULL) {
        next = current->next;
        current->name[0] = '\0';
        for(i = 0; i < current->used_counter; i++) {
            current->used_addresses[i] = 0;
        }
        current->used_counter = 0;
        free(current->next);
        current->next = NULL;
        current = next;
    }

    head = NULL;
}
