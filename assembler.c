#include "MacroProcessing.h"
#include "FirstPass.h"
#include "SecondPass.h"
#include "output.h"

int main(int argc, char ** argv){
    int i;
    char * amFileName = NULL;
    struct MacroContext macro_table;
    FILE *am_file = NULL;

    /* Iterate over input parameters */
    for(i = 1; i < argc; i++){
        amFileName = macro_processing(argv[i], &macro_table); /* Create am file*/
        if(amFileName){ /* If file created successfully without errors */
            am_file = open_file(amFileName, "r"); /* Open am file for read mode */
            if(am_file){
                if(firstPass(amFileName, am_file) != 1){ /* Run first pass */
                    /* Firstpass success */
                    if(secondPass(amFileName, am_file) != 1){ /* Run second pass */
                        /* Secondpass success */
                        createObFile(amFileName); /* Create ob file */
                        createEntFile(amFileName); /* Create ent file */
                        createExtFile(amFileName); /* Create ext file */
                    }
                }
                fclose(am_file);
            }
            free(amFileName);
            /*free(machine_code_ptr);*/
            free_macro_table(&macro_table);
            /*free(extern_usage);*/
            /*free(machine_code);*/
        }
    }

    return 0;
}