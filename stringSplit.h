#ifndef STRINGSPLIT_H
#define STRINGSPLIT_H

#include <string.h>
#include <ctype.h>
#include "constants.h"

/* Structure to hold the splitted string */
struct string_split{
    char * string[81];
    int size;
};

#include "helpingFunction.h"

/* Functions Prototypes */
struct string_split split_string(char * str, const char * delimiter);

#endif
