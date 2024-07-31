#include "FirstPass.h"

int firstPass(char * file_name, FILE * file) {
    
    /* Declarations */
    int error_flag = 0;
    int IC = 0; 
    int DC = 0;
    int L = 0; /* Number of words that the current instruction takes */
    int line_counter = 1; /* The number of line i just read from (am file) */
    char read_line[MAX_LINE_LENGTH];
    char buffer_line[MAX_BUFFER_LENGTH]; 
    struct ast answer = {0}; /* After front returned answer*/
    table_ptr head_ptr = NULL; /* The poiner to the head of the table*/
    table_ptr found = NULL; /* Recive the address of the symbol inside the table*/
    int i;

    /* Read lines from the am file */
    while(fgets(buffer_line,sizeof(buffer_line),file)) {

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
        if((answer.labelName != '\0') && ((answer.ast_type == ast_inst) || answer.ast_type == ast_dir)) {
            
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
                        else if((answer.ast_options.dir.dir_type == ast_entry) || answer.ast_options.dir.dir_type == ast_extern) {
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
                
                /* If its an inst */    /*i need to check how to insert, it will instert all at the same DC*/
                if(answer.ast_type == ast_inst) {
                    if(IC == 0) {
                        IC = 100;
                    }
                    add_symbol_to_table(answer.labelName, answer.ast_type, IC, &head_ptr);

                    /* Calculate how many arguments there is */
                    L = 0;
                    for (i = 0; i < 2; i++) {
                        if (answer.ast_options.inst.operands[i].operand_type != ast_none) {
                            L++;
                        }
                    }
                    IC += L;
                }

                /* If its a dir */
                if(answer.ast_type == ast_dir) {

                    /* If its external variable */      /*need to check if its zero or NULL*/
                    if(answer.ast_options.dir.dir_type == ast_extern) {
                        add_symbol_to_table(answer.labelName, answer.ast_type, 0, &head_ptr);
                    }

                    /* If its not external variable */      /*i need to check how to insert, it will instert all at the same DC*/
                    else {
                        ++DC;
                        add_symbol_to_table(answer.labelName, answer.ast_type, DC, &head_ptr);
                        L = answer.ast_options.dir.dir_options.data_size;
                        DC += L;
                    }
                }
            }
        }
        ++line_counter;
    }

    /* Check if there is entry without defeniton */
    found = head_ptr;
    while(found) {
        if(found -> symbol_type == entry_symbol) {
            printf("Error: In file %s symbol %s declared as entry but never defined.\n", file_name, found -> symbol_name);
            error_flag = 1;
        }
        found = found -> next;

        /* Relocate all DC variables after IC variables*/
        if((found -> symbol_type == data_symbol) || (found -> symbol_type == entry_data)) {
            found -> symbol_address += IC;
        }
    }

    return error_flag;
}
