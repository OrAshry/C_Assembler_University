#include <stdio.h>

enum FunctionId
{
    CALLOC_ID,
    MALLOC_ID,
    REALLOC_ID
};

void *allocateMemory(size_t numElements, size_t sizeOfElement, int functionID);