#include "output.h"

/**
 * @brief Creates the entry file (.ent) for the given input file.
 *
 * This function generates a .ent file containing all the entry symbols from the symbol table
 * and their corresponding addresses. The file is named after the input file with a .ent extension.
 *
 * @param input_file_name The name of the original input file (without the .ent extension).
 */
void createEntFile(const char *input_file_name)
{
    char *ent_file_name;
    FILE *ent_file;
    table_ptr find = head_ptr;
    table_ptr current_entry;

    /* Check if there are entry symbols */
    if(find_entry(find) == NULL)
    {
        return;
    }

    /* Allocate memory for the .ent file name */
    ent_file_name = (char *)allocateMemory(1, strlen(input_file_name) + 5, MALLOC_ID);

    /* Create the .ent file name */
    strcpy(ent_file_name, input_file_name);
    strcat(ent_file_name, ".ent");

    /* Open .ent file for writing */
    ent_file = fopen(ent_file_name, "w");
    if(!ent_file)
    {
        fprintf(stderr, "Could not open the file %s for writing\n", ent_file_name);
        free(ent_file_name);
        return;
    }

    /* Writing the symbol names and addresses */
    while ((current_entry = find_entry(find)) != NULL)
    {
        find = current_entry->next;

        /* Write the symbol name and address */
        fprintf(ent_file, "%s %d", current_entry->symbol_name, current_entry->symbol_address);

        /* Check if this is the last entry */
        if (find_entry(find) != NULL)
        {
            fprintf(ent_file, "\n");
        }
    }

    fclose(ent_file);
    free(ent_file_name);
}

/**
 * @brief Creates the extern file (.ext) for the given input file.
 *
 * This function generates a .ext file containing all the external symbols from the symbol table
 * and their usage addresses. The file is named after the input file with a .ext extension.
 *
 * @param input_file_name The name of the original input file (without the .ext extension).
 * @param extern_usage_head_ptr A pointer to the head of the linked list containing extern usage information.
 */
void createExtFile(const char *input_file_name, extern_addresses_ptr extern_usage_head_ptr)
{
    char *ext_file_name;
    FILE *ext_file;
    table_ptr find = head_ptr;
    extern_addresses_ptr current_extern = extern_usage_head_ptr;
    int i;

    /* Check if there are extern symbols */
    if (!find_extern_in_symbol_table(find))
    {
        return;
    }

    /* Allocate memory for the .ext file name */
    ext_file_name = (char *)allocateMemory(1, strlen(input_file_name) + 5, MALLOC_ID);

    /* Create the .ext file name */
    strcpy(ext_file_name, input_file_name);
    strcat(ext_file_name, ".ext");

    /* Open .ext file for writing */
    ext_file = fopen(ext_file_name, "w");
    if (!ext_file)
    {
        fprintf(stderr, "Could not open the file %s for writing\n", ext_file_name);
        free(ext_file_name);
        return;
    }

    /* Writing the symbol names and the addresses they have been used */
    while (current_extern != NULL)
    {
        /* Write each address */
        for (i = 0; i < current_extern->used_counter; i++)
        {
            fprintf(ext_file, "%s\t%04d", current_extern->name, current_extern->used_addresses[i]);
            /* Avoid adding a newline if it's the last entry */
            if (current_extern->next != NULL || i < current_extern->used_counter - 1)
            {
                fprintf(ext_file, "\n");
            }
        }
        current_extern = current_extern->next;
    }

    /* Clean up */
    fclose(ext_file);
    free(ext_file_name);
}

/**
 * @brief Creates the object file (.ob) for the given input file.
 *
 * This function generates a .ob file containing the assembled machine code and data image.
 * The file is named after the input file with a .ob extension and includes the number of instructions
 * and directives.
 *
 * @param input_file_name The name of the original input file (without the .ob extension).
 */
void createObFile(const char *input_file_name) {
    char *ob_file_name;
    FILE *ob_file;

    /* Check if there is any code  */
    if((machine_code_ptr->DC == 0) && (machine_code_ptr->IC == 0))
    {
        return;
    }

    /* Allocate memory for the .ob file name */
    ob_file_name = (char *)allocateMemory(1, strlen(input_file_name) + 4, MALLOC_ID);

    /* Create the .ob file name */
    strcpy(ob_file_name, input_file_name);
    strcat(ob_file_name, ".ob");

    /* Open .ob file for writing */
    ob_file = fopen(ob_file_name, "w");
    if (!ob_file) 
    {
        fprintf(stderr, "Could not open the file %s for writing\n", ob_file_name);
        free(ob_file_name);
        return;
    }

    /* Writing the header (number of instructions and number of directive) */
    if((machine_code_ptr->IC) == 0) 
    {
        fprintf(ob_file, "%d\t%d\n", (machine_code_ptr->IC),  (machine_code_ptr->DC));
    }
    else 
    {
        fprintf(ob_file, "%d\t%d\n", (machine_code_ptr->IC) - 100,  (machine_code_ptr->DC));
    }
    /* Writing the code_image */
    fprint_code_image(machine_code_ptr, ob_file);
    fprint_data_image(machine_code_ptr, ob_file);
    
    /* Clean up */
    fclose(ob_file);
    free(ob_file_name);
}
