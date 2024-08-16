#ifndef STRINGSPLIT_H
#define STRINGSPLIT_H

#include "stringSplit.h"
#include <string.h>
#include <ctype.h>
#include "Constants.h"

struct string_split{
    char * string[81];
    int size;
};

struct string_split split_string(char * str, const char * delimiter);

#endif