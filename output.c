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

    /* allocate memory for the .ent file */
    ent_file_name = (char *)allocateMemory(1, strlen(input_file_name) + 5, MALLOC_ID);

    /* Create the .ent file name */
    strcpy(ent_file_name, input_file_name);
    strcat(ent_file_name, ".ent");

    /* Open .ent file for writing */
    ent_file = fopen("ent_file_name", "w");
    if(!ent_file) {
        fprintf(stderr, "Could not open the file %s for writing\n", ent_file_name);
        free(ent_file);
        return;
    }

    /* Writing the symbol names and addresses*/
    while((find = find_entry(find)) != NULL) {
        fprintf(ent_file, "%s %d\n", find -> symbol_name, find -> symbol_address);
        find = find -> next;
    }

    fclose(ent_file);
    free(ent_file_name);
}
