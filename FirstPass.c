#include "FirstPass.h"

int firstPass(char * file_name, FILE * file) {
    
    /* Declarations */
    int error_flag = 0;
    int IC = 0; /* need to check if its starts from 0 or 100*/
    int DC = 0; /* need to check if it starts from 0 or 100*/
    int L = 0; /* Number of words that the current instruction takes */
    int line_counter = 0; /* The number of line i just read from (am file) */
    char read_line[MAX_LINE_LENGTH];
    struct ast answer = {0}; /* After front returned answer*/
    table_ptr p1; /* The poiner to the symbol table*/

    /* Read lines from the am file */
    while(fgets(read_line,sizeof(read_line),am_file)) {
        answer = front_answer(read_line);
        
        /* If there is a syntax error*/
        if(answer.lineError[0] != '\0') {
            printf("Error: In file %s at line %d there is a sysntax error: %s\n", am_file,line_counter,answer.lineError);
            line_counter++;
            error_flag = 1;
            continue;
        }

        /* If there is a symbol in the line */
        if((answer.labelName != null) && ((answer.ast_type == data) || answer.ast_type == string)) {
            
            /* If the symbol is already exist in the table */
            if(symbol_search(p1, answer.labelName)) {
                
                /* If entry*/
                if(answer.ast_type == entry) {
                    ;
                }
                
                /* If its not an entry*/
                else {
                    printf("Error: In file %s at line %d the symbol %s has been redefined.\n", am_file,line_counter, answer.labelName);
                    line_counter++;
                    error_flag = 1;
                }
            }

            /* If the symbol is not in the table*/
            else {
                
                /* If its IC */
                add_symbol_to_table(answer.labelName, answer.ast_type, IC, p1);

                /* If its DC*/
                add_symbol_to_table(answer.labelName, answer.ast_type, DC, p1);
            }
        }

    }

    return error_flag;
}