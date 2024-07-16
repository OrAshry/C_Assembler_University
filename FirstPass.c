include "FirstPass.h"

int firstPass(char * file_name, FILE * file) {
    /* Declarations */
    int error_flag = 0;
    int IC = 0; /* need to check if its starts from 0 or 100*/
    int DC = 0; /* need to check if it starts from 0 or 100*/
    int L = 0;
    int line_counter = 1; 
    char read_line[MAX_LINE_LENGTH];
    struct aft line = {0}; /* After front translation line*/

    /* Read lines from the am file */
    while(fgets(read_line,sizeof(read_line),am_file)) {
        aft line = front_trans(read_line);
    }

    return error_flag;
}