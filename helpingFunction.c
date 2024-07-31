#include <stdio.h>
#include "helpingFunction.h"
#include <stdlib.h>

void *allocateMemory(size_t numElements, size_t sizeOfElement, int functionID)
{
    void *ptr;
    switch (functionID)
    {
    case MALLOC_ID:
        ptr = malloc(numElements * sizeOfElement);
        break;
    case CALLOC_ID:
        ptr = calloc(numElements, sizeOfElement);
        break;
    }

    if (ptr == NULL)
    {
        printf("Error: Unable to allocate memory\n");
        exit(0);
        /*continue to next file*/
    }
    return ptr;
}