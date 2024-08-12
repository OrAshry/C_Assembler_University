#include "output.h"

void createEntFile(const char *source_file_name, int num_of_symbols) {
    FILE *file;
    file = fopen(source_file_name, "r");
    found = head_ptr;
    char *symbol_name;
    int i;

    for(i = 0; i < num_of_symbols; i++)
        found = find_entry(head_ptr);
    

    fclose(source_file_name);
}
