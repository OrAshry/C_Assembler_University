#include "FirstPass.h"

int firstPass(char *file_name, FILE *file) {

    /* Declarations */
    int error_flag = 0;
    int IC = 0; /* need to check if it starts from 0 or 100 */
    int DC = 0; /* need to check if it starts from 0 or 100 */
    int L = 0; /* Number of words that the current instruction takes */
    int line_counter = 1; /* The number of line i just read from (am file) */
    table_ptr head_ptr = NULL; /* The pointer to the symbol table */
    table_ptr found = NULL; /* Receive the address of the symbol inside the table */
    translation program;
    translation_ptr program_ptr = &program;
    int i;

    /* This is a Test */
    struct ast node = {
        .lineError = "No error",
        .labelName = "myLabel",
        .ast_type = ast_inst,
        .ast_options = {
            .inst = {
                .inst_type = ast_add,
                .operands = {
                    {
                        .operand_type = ast_immidiate,
                        .operand_option = {.immed = 10}
                    },
                    {
                        .operand_type = ast_register_direct,
                        .operand_option = {.reg = 2}
                    }
                }
            }
        }
    };

    /* Using the node directly */
    struct ast answer = node;

    /* If there is a syntax error */
    if(answer.ast_type == ast_error) {
        printf("Error: In file %s at line %d there is a syntax error: %s\n", file_name, line_counter, answer.lineError);
        error_flag = 1;
    }

    /* If there is a symbol in the line */
    if((answer.labelName[0] != '\0') && ((answer.ast_type == ast_inst) || answer.ast_type == ast_dir)) {

        /* If the symbol already exists in the table */
        if((found = symbol_search(head_ptr, answer.labelName))) {

            /* If the symbol in the table is entry */
            if(found->symbol_type == entry_symbol) {

                /* If the symbol in the line is inst */
                if(answer.ast_type == ast_inst) {
                    found->symbol_type = entry_code;
                }

                /* If the symbol in the line is dir */
                else if(answer.ast_type == ast_dir) {

                    /* If it's data or string */
                    if((answer.ast_options.dir.dir_type == ast_data) || (answer.ast_options.dir.dir_type == ast_string)) {
                        found->symbol_type = entry_data;
                    }

                    /* If it's entry or extern */
                    else if((answer.ast_options.dir.dir_type == ast_entry) || answer.ast_options.dir.dir_type == ast_extern) {
                        printf("Error: In file %s at line %d the symbol %s has been redefined.\n", file_name, line_counter, answer.labelName);
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

            /* If the symbol in the table is not entry */
            else {
                printf("Error: In file %s at line %d the symbol %s has been redefined.\n", file_name, line_counter, answer.labelName);
                error_flag = 1;
            }
        }

        /* If the symbol is not in the table */
        else {

            /* If it's an inst */ /* I need to check how to insert, it will insert all at the same DC */
            if(answer.ast_type == ast_inst) {
                if(IC == 0) {
                    IC = 100;
                }
                add_symbol_to_table(answer.labelName, answer.ast_type, IC, &head_ptr);
            }

            /* If it's a dir */
            if(answer.ast_type == ast_dir) {

                /* If it's an external variable */ /* Need to check if it's zero or NULL */
                if(answer.ast_options.dir.dir_type == ast_extern) {
                    add_symbol_to_table(answer.labelName, answer.ast_type, 0, &head_ptr);
                }

                /* If it's not an external variable */ /* I need to check how to insert, it will insert all at the same DC */
                else {
                    ++DC;
                    add_symbol_to_table(answer.labelName, answer.ast_type, DC, &head_ptr);
                }
            }
        }

        /* Calculate words if its inst*/
        if(answer.ast_type = ast_inst) {

            for (i = 0; i < 2; i++) {
                if (answer.ast_options.inst.operands[i].operand_type != ast_none) {
                    L++;
                }
            }
            IC += L;
        }

        /* Calculate words and code them into data_image*/
        else if((answer.ast_type == ast_dir) && ((answer.ast_options.dir.dir_type == ast_data) || answer.ast_options.dir.dir_type == ast_string)) {
            memcpy(&program_ptr -> data_image[DC], answer.ast_options.dir.dir_options.data, answer.ast_options.dir.dir_options.data_size);
            L = answer.ast_options.dir.dir_options.data_size;
            DC += L;
        }

    }

    /* Check if there is entry without defeniton */
    found = head_ptr;
    while (found) {
        if (found->symbol_type == entry_symbol) {
            printf("Error: In file %s symbol %s declared as entry but never defined.\n", file_name, found->symbol_name);
            error_flag = 1;
        }
        found = found -> next;
    }

    /* Relocate all DC variables after IC variables*/
    found = head_ptr;
    while(found) {
        if((found -> symbol_type == data_symbol) || (found -> symbol_type == entry_data)) {
            found -> symbol_address += IC;
        }
        found = found -> next;
    }

    print_symbol_table(head_ptr);

    return error_flag;
}

    int main(void) {
    /* Initialize a test file and FILE pointer */
    FILE *file = fopen("test.am", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    /* Call firstPass function with a test file name and the file pointer */
    int result = firstPass("test.am", file);

    /* Close the file */
    fclose(file);

    /* Print result (or any other output you expect) */
    printf("Result: %d\n", result);

    return 0;
}
