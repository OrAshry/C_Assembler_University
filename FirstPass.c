include "FirstPass.h"

define MAX_LINE_LENGTH 81

int firstPass(char * file_name, FILE * file) {
    /* Declarations */
    int error_flag = 0;
    int IC = 0; /* need to check if its starts from 0 or 100*/
    int DC = 0; /* need to check if it starts from 0 or 100*/
    int L = 0;
    int line_counter = 1; 
    char read_line[MAX_LINE_LENGTH];
    struct aft line = {0}; /* After front translation*/ 

    return error_flag;
}