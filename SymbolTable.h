#include <stdio.h>

#define MAX_NAME 31

typedef struct symbol-table * ptr;
typedef struct symbol-table {
    char name[MAX_NAME];
    int address;
    ptr next;
};
