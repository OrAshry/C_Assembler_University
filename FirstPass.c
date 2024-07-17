#include "FirstPass.h"

int firstPass(char * file_name, FILE * file) {
    /* Declarations */
    int error_flag = 0;
    int IC = 0; /* need to check if its starts from 0 or 100*/
    int DC = 0; /* need to check if it starts from 0 or 100*/
    int L = 0;
    int line_counter = 1; 
    char read_line[MAX_LINE_LENGTH];
    struct afr line = {0}; /* After front returned line*/

    /* Read lines from the am file */
    while(fgets(read_line,sizeof(read_line),am_file)) {
        line = front_answer(read_line);
        if(line.error == 1) {
            printf("In file %s at line %d there is a sysntax error: %s", am_file,L,line.error);
            L++;
            error_flag =1
            continue;
        }
        





    }

    return error_flag;
}