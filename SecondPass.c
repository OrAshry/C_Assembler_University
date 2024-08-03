#include "SecondPass.h"

int secondPass(char * file_name, FILE * file) {
    
    /* Declarations */
    int error_flag = 0;
    char line[MAX_LINE_LENGTH];
    struct ast answer_line = {0};

    while(fgets(line, MAX_LINE_LENGTH, file)) {
        answer_line = get_ast_from_line(line);

        if((answer_line.ast_type == ast_dir) && ((answer_line.ast_options.dir.dir_type == ast_data) || (answer_line.ast_options.dir.dir_type == ast_string))) {
            ;
        }
    }

    return error_flag;
}
