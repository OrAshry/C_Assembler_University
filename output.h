#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include "FirstPass.h"
#include "SecondPass.h"

/* Prototypes */
void createEntFile(const char *input_file_name);
void createExtFile(const char *input_file_name,  extern_addresses_ptr extern_usage_head_ptr);
void createObFile(const char *input_file_name);

#endif
