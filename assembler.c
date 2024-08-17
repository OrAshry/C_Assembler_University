#include "macroProcessing.h"
#include "firstPass.h"
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
                if(firstPass(argv[i], am_file) != 1){ /* Run first pass */
                    /* Firstpass success */
                    rewind(am_file); /* Rewind file pointer */
                    if(secondPass(argv[i], am_file) != 1){ /* Run second pass */
                        /* Secondpass success */
                        createEntFile(argv[i]); /* Create ent file */
                        createExtFile(argv[i]); /* Create ext file */
                        createObFile(argv[i]); /* Create ob file */
                    }
                }
                fclose(am_file); /* Close file */
            }
            free(amFileName);
            free_macro_table(&macro_table);
        }
    }

    return 0;
}