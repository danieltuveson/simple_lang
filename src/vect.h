#ifndef VECT
#define VECT
#include "stdincludes.h"
#include <stddef.h>

struct Vect 
{
    size_t size;
    size_t capacity;
    void **array;
};

#include "vect.c.generated.h"
#endif
