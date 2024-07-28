#include "FirstPass.h"

int firstPass(char * file_name, FILE * file) {
    
    /* Declarations */
    int error_flag = 0;
    int IC = 0; /* need to check if its starts from 0 or 100*/
    int DC = 0; /* need to check if it starts from 0 or 100*/
    int L = 0; /* Number of words that the current instruction takes */
    int line_counter = 1; /* The number of line i just read from (am file) */
    char read_line[MAX_LINE_LENGTH];
    struct ast answer = {0}; /* After front returned answer*/
    table_ptr p1 = NULL; /* The poiner to the symbol table*/
    table_ptr found = NULL; /* Recive the address of the symbol inside the table*/
    int i;

    /* Read lines from the am file */
    while(fgets(read_line,sizeof(read_line),file)) {
        answer = get_ast_from_line(read_line);
        
        /* If there is a syntax error*/
        if(answer.ast_type == ast_error) {
            printf("Error: In file %s at line %d there is a sysntax error: %s\n", file_name,line_counter,answer.lineError);
            line_counter++;
            error_flag = 1;
            continue;
        }

        /* If there is a symbol in the line */
        if((answer.labelName != NULL) && ((answer.ast_type == ast_inst) || answer.ast_type == ast_dir)) {
            
            /* If the symbol is already exist in the table */
            if((found = symbol_search(p1, answer.labelName))) {
                
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

                /* If the symbol in the table is not entry*/
                else {
                    printf("Error: In file %s at line %d the symbol %s has been redefined.\n", file_name,line_counter, answer.labelName);
                    error_flag = 1;
                }
            }

            /* If the symbol is not in the table*/
            else {
                
                /* If its an inst */ /*i need to check how to insert, it will instert all at the same DC*/
                if(answer.ast_type == ast_inst) {
                    if(IC == 0) {
                        IC = 100;
                    }
                    add_symbol_to_table(answer.labelName, answer.ast_type, IC, &p1);

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

                    /* If its external variable */ /*need to check if its zero or NULL*/
                    if(answer.ast_options.dir.dir_type == ast_extern) {
                        add_symbol_to_table(answer.labelName, answer.ast_type, 0, &p1);
                    }

                    /* If its not external variable */ /*i need to check how to insert, it will instert all at the same DC*/
                    else {
                        ++DC;
                        add_symbol_to_table(answer.labelName, answer.ast_type, DC, &p1);
                        L = answer.ast_options.dir.dir_options.data_size;
                        DC += L;
                    }
                }
            }
        }
        ++line_counter;
    }

    return error_flag;
}

int main(void) {
    char *file_name = "test.am";
    FILE *file = fopen(file_name, "r");
    
    if (file == NULL) {
        printf("Error: Could not open file %s\n", file_name);
        return 1;
    }

    int result = firstPass(file_name, file);
    
    fclose(file);

    if (result == 0) {
        printf("First pass completed successfully with no errors.\n");
    } else {
        printf("First pass completed with errors.\n");
    }

    return result;
}
