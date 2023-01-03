#ifndef VECT
#define VECT
#include "stdincludes.h"
#include <stddef.h>

struct Vect 
{
    size_t size; // number of items currently in array
    size_t capacity; // number of items array could contain
    void **array;
};

#include "vect.c.generated.h"
#endif
