#include <stdio.h>
#include "FirstPass.h"

typedef struct symboltable * ptr;
typedef struct symboltable {
    char name[MAX_SYMBOL_NAME]
    int address;
    enum symbol_type;
    ptr next;
};
