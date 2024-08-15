#include "output.h"

/* Entry's output file */
void createEntFile(const char *input_file_name) {
    char *ent_file_name;
    FILE *ent_file;
    table_ptr find = head_ptr;

    /* Check if there are entry symbols */
    if(find_entry(find) == NULL) {
        return;
    }

    /* Allocate memory for the .ent file name */
    ent_file_name = (char *)allocateMemory(1, strlen(input_file_name) + 5, MALLOC_ID);

    /* Create the .ent file name */
    strcpy(ent_file_name, input_file_name);
    strcat(ent_file_name, ".ent");

    /* Open .ent file for writing */
    ent_file = fopen(ent_file_name, "w");
    if(!ent_file) {
        fprintf(stderr, "Could not open the file %s for writing\n", ent_file_name);
        free(ent_file_name);
        return;
    }

    /* Writing the symbol names and addresses */
    while((find = find_entry(find)) != NULL) {
        fprintf(ent_file, "%s %d\n", find -> symbol_name, find -> symbol_address);
        find = find -> next;
    }

    fclose(ent_file);
    free(ent_file_name);
}

/* Extern output file */
void createExtFile(const char *input_file_name) {
    char *ext_file_name;
    FILE *ext_file;
    table_ptr find = head_ptr;
    extern_addresses_ptr current_extern = extern_usage_head_ptr;
    int i;

    /* Check if there are extern symbols */
    if (!find_extern_in_symbol_table(find)) {
        return;
    }

    /* Allocate memory for the .ext file name */
    ext_file_name = (char *)allocateMemory(1, strlen(input_file_name) + 5, MALLOC_ID);

    /* Create the .ext file name */
    strcpy(ext_file_name, input_file_name);
    strcat(ext_file_name, ".ext");

    /* Open .ext file for writing */
    ext_file = fopen(ext_file_name, "w");
    if (!ext_file) {
        fprintf(stderr, "Could not open the file %s for writing\n", ext_file_name);
        free(ext_file_name);
        return;
    }

    /* Writing the symbol names and the addresses they have been used */
    while (current_extern != NULL) {
        /* Write each address on a new line */
        for (i = 0; i < current_extern->used_counter; i++) {
            fprintf(ext_file, "%s\t%d\n", current_extern->name, current_extern->used_addresses[i]);
        }
        current_extern = current_extern->next;
    }

    /* Clean up */
    fclose(ext_file);
    free(ext_file_name);
}
