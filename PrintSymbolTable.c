/*for the test*/
#include "SymbolTable.h"

void print_symbol_table(table_ptr p) {
    printf("Symbol Table:\n");
    while (p != NULL) {
        printf("Name: %-15s Type: %-15d Address: %-5d Next : 0d\n",
               p->symbol_name, p->symbol_type, p->symbol_address, p->next);
        p = p->next;
    }
}
