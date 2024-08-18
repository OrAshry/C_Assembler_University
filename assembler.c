#include "macroProcessing.h"
#include "firstPass.h"
#include "SecondPass.h"
#include "output.h"

int main(int argc, char **argv)
{
    int i, err_flag = 0;
    char *amFileName = NULL;
    struct MacroContext macro_table;
    extern_addresses extern_usage = {0};
    FILE *am_file = NULL;

    /* Iterate over input parameters */
    for (i = 1; i < argc; i++)
    {
        amFileName = macro_processing(argv[i], &macro_table); /* Create am file*/
        if (amFileName)
        {   /* If file created successfully without errors */
            am_file = open_file(amFileName, "r"); /* Open am file for read mode */
            if (am_file)
            {
                if (firstPass(argv[i], am_file, &macro_table) != 1)
                { /* Run first pass */
                    /* Firstpass success */
                    rewind(am_file); /* Rewind file pointer */
                    extern_usage = *(secondPass(argv[i], am_file, &extern_usage, &err_flag));
                    if (err_flag != 1)
                    { /* Run second pass */
                        /* Secondpass success */
                        createEntFile(argv[i]); /* Create ent file */
                        createExtFile(argv[i], &extern_usage); /* Create ext file */
                        createObFile(argv[i]);  /* Create ob file */
                    }
                }
                fclose(am_file); /* Close file */
            }
            /* Freeing variables */
            free(amFileName);
            free_macro_table(&macro_table);
            free_symbol_table(&head_ptr);
            free_extern_table(&extern_usage);
            free_machine_code(machine_code_ptr);
        }
    }

    return 0;
}
