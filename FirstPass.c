#include "FirstPass.h"

int firstPass(char * file_name, FILE * file, table_ptr p) {
    /* Declarations */
    int error_flag = 0;
    int IC = 0; /* need to check if its starts from 0 or 100*/
    int DC = 0; /* need to check if it starts from 0 or 100*/
    int L = 0; /* Number of words that the current instruction takes */
    int line_counter = 0; /* The number of line i just read from (am file) */
    char read_line[MAX_LINE_LENGTH];
    struct ast answer = {0}; /* After front returned answer*/
    symbol_ptr p1;

    /* Read lines from the am file */
    while(fgets(read_line,sizeof(read_line),am_file)) {
        answer = front_answer(read_line);
        
        /* If there is a syntax error*/
        if(line.lineError[0] != '\0') {
            printf("In file %s at line %d there is a sysntax error: %s", am_file,line_counter,line.lineError);
            line_counter++;
            error_flag = 1;
            continue;
        }

        /* If there is a symbol in the line */
        if((line.symbol == 1) && ((line.type == data) || line.type == string)) {
            
            /* If the symbol is already exist in the table*/
            if(symbol_search()) {
                
            }

            /* If the symbol is not in the table*/
            else {
                add_symbol_to_table();
            }
        }

        





    }

    return error_flag;
}