#include "FirstPass.h"

int firstPass(char * file_name, FILE * file) {
    
    /* Declarations */
    int error_flag = 0;
    int L; /* Number of words that the current instruction takes */
    int i;
    int line_counter = 1; /* The line number of the source file after macro */
    char read_line[MAX_LINE_LENGTH];
    char buffer_line[MAX_BUFFER_LENGTH]; 
    struct ast answer = {0}; /* After front returned answer*/
    table_ptr head_ptr = NULL; /* The poiner to the head of the table*/
    table_ptr found = NULL; /* Recive the address of the symbol inside the table*/
    translation machine_code = {0};
    translation_ptr machine_code_ptr = &machine_code;

    /* Read lines from the am file */
    while(fgets(buffer_line, MAX_BUFFER_LENGTH, file)) {

        /* Checks if the line from source code is longer than 80 */
        if((strlen(buffer_line) > MAX_LINE_LENGTH - 1) && (buffer_line[MAX_LINE_LENGTH - 1] != '\n')) {
            printf("Error: In file %s at line %d, the line exceeds 80 characters.\n", file_name, line_counter);
            line_counter++;
            error_flag = 1;
            continue;
        }
        strcpy(read_line, buffer_line);
        answer = get_ast_from_line(read_line);
        
        /* If there is a syntax error*/
        if(answer.ast_type == ast_error) {
            printf("Error: In file %s at line %d there is a sysntax error: %s\n", file_name,line_counter,answer.lineError);
            line_counter++;
            error_flag = 1;
            continue;
        }

        /* If there is a symbol in the line */
        if((answer.labelName[0] != '\0') && ((answer.ast_type == ast_inst) || answer.ast_type == ast_dir)) {
            
            /* If the symbol is already exist in the table */
            if((found = symbol_search(head_ptr, answer.labelName))) {
                
                /* If the symbol in the table is entry*/
                if(found -> symbol_type == entry_symbol) {
                    
                    /* If the symbol in the line is inst */
                    if(answer.ast_type == ast_inst) {
                        found -> symbol_type = entry_code;
                    }
                    
                    /* If the symbol in the line is dir */
                    else if(answer.ast_type == ast_dir) {
                        
                        /* If its data or string */
                        if((answer.ast_options.dir.dir_type == ast_data) || (answer.ast_options.dir.dir_type == ast_string)) {
                            found -> symbol_type = entry_data;
                        }
                        
                        /* If its entry or extern */
                        else {
                            printf("Error: In file %s at line %d the symbol %s has been redefined.\n", file_name,line_counter, answer.labelName);
                            error_flag = 1;
                        }
                    }
                }

                /* If the symbol in the table is daclared and they cange it to entry after that*/
                else if((answer.ast_type == ast_dir) && (answer.ast_options.dir.dir_type == ast_entry)) {
                    if(found -> symbol_type == code_symbol) {
                        found -> symbol_type = entry_code;
                    }
                    else if(found -> symbol_type == data_symbol) {
                        found -> symbol_type = entry_data;
                    }
                }

                /* If the symbol in the table is not entry*/
                else {
                    printf("Error: In file %s at line %d the symbol %s has been redefined.\n", file_name,line_counter, answer.labelName);
                    error_flag = 1;
                }
            }

            /* If the symbol is not in the table*/
            else {
                
                /* If its an inst */
                if(answer.ast_type == ast_inst) {
                    if((machine_code_ptr -> IC) == 0) {
                        (machine_code_ptr -> IC) = 100;
                        add_symbol_to_table(answer.labelName, code_symbol, (machine_code_ptr -> IC), &head_ptr);
                    }
                    else {
                        add_symbol_to_table(answer.labelName, code_symbol, (machine_code_ptr -> IC), &head_ptr);
                    }
                }

                /* If its a dir */
                if(answer.ast_type == ast_dir) {

                    /* If its external variable */      /*need to check if its zero or NULL*/
                    if(answer.ast_options.dir.dir_type == ast_extern) {
                        add_symbol_to_table(answer.labelName, extern_symbol, 0, &head_ptr);
                    }

                    /* If its entery variable */
                    else if(answer.ast_options.dir.dir_type == ast_entry) {
                        ++(machine_code_ptr -> DC);
                        add_symbol_to_table(answer.labelName, entry_symbol,  (machine_code_ptr -> DC), &head_ptr);
                    }

                    /* If its data or string */
                    else {
                        ++(machine_code_ptr -> DC);
                        add_symbol_to_table(answer.labelName, data_symbol, (machine_code_ptr -> DC), &head_ptr);
                    }
                }
            }
        }

        /* Calculate words if its inst variable*/
        if(answer.ast_type == ast_inst) {
            L = 1; /* Initialize the word counter for inst */
            for (i = 0; i < 2; i++) {
                printf("Operand %d type: %d\n", i, answer.ast_options.inst.operands[i].operand_type);
                if (answer.ast_options.inst.operands[i].operand_type != ast_none) {
                    L++;
                }
            }
            
            /* if there are 2 operands both registers */
            if(L == 3) {
                if(((answer.ast_options.inst.operands[0].operand_type == ast_register_direct) || (answer.ast_options.inst.operands[0].operand_type == ast_register_address)) &&
                 ((answer.ast_options.inst.operands[1].operand_type == ast_register_direct) || (answer.ast_options.inst.operands[1].operand_type == ast_register_address))) {
                    L--;
                }
            }
            (machine_code_ptr -> IC) += L;
            printf("IC is %d\n", machine_code_ptr -> IC);
        }

        /* Calculate words if its dir variable */
        else if(answer.ast_type == ast_dir) {
            L = 0;
            if((answer.ast_options.dir.dir_type == ast_data) || (answer.ast_options.dir.dir_type == ast_string)) {
                L = answer.ast_options.dir.dir_options.data_size - 1;/* -1 becasue the first char got storage when declarated */
            }
            (machine_code_ptr -> DC) += + L;
            printf("DC is = %d\n", machine_code_ptr -> DC);
        }
         


        ++line_counter;
    }

    /* Check if there is entry without defeniton */ /* in the assembler example they say that if there is a proble m to exit here*/
    found = head_ptr;
    while(found) {
        if(found -> symbol_type == entry_symbol) {
            printf("Error: In file %s symbol %s declared as entry but never defined.\n", file_name, found -> symbol_name);
            error_flag = 1;
            return error_flag;
        }

        /* Relocate all DC variables after IC variables*/
        else if((found -> symbol_type == data_symbol) || (found -> symbol_type == entry_data)) {
            (found -> symbol_address) += (machine_code_ptr -> IC);
        }
        found = (found -> next);
    }
    
    print_symbol_table(head_ptr);
    
    return error_flag;
}

int main(void) {
    FILE *file = NULL;
    int x;
    /*read my file test.am*/
    file = fopen("test.am", "r");
    if(file == NULL) {
        return 1;
    }
    x = firstPass("test.am", file);
    printf("the error flag is %s\n", x ? "on" : "off");
    fclose(file);
    return x;
}
