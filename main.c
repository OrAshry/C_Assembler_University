#include "FirstPass.h"

void print_symbol_table(table_ptr head) {
    table_ptr current = head;
    printf("\nSymbol Table:\n");
    printf("Name\t\tType\t\tAddress\n");
    while (current != NULL) {
        printf("%s\t\t%d\t\t%d\n", current->symbol_name, current->symbol_type, current->symbol_address);
        current = current->next;
    }
}

int main() {
    // Test the firstPass function and print the symbol table
    FILE *file = fopen("test.am", "r");
    if (file == NULL) {
        printf("Error: File not found\n");
        return 1;
    }

    // Initialize the symbol table pointer
    table_ptr symbol_table = NULL;

    // Call firstPass
    int error_flag = firstPass("test.am", file);
    fclose(file);

    // Print the symbol table
    print_symbol_table(symbol_table);

    return error_flag;
}
