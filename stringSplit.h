#ifndef STRINGSPLIT_H
#define STRINGSPLIT_H

#include <string.h>
#include <ctype.h>
#include "constants.h"

struct string_split{
    char * string[81];
    int size;
};

#include "helpingFunction.h"

/* Split a string into substrings using a delimiter */
struct string_split split_string(char * str, const char * delimiter);

#endif
