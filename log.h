#ifndef LOG
#define LOG

#include "datatype.h"
#include "lexer.h"
#include "parser.h"

// IS_DEBUG 1: enables printing statements for debugging
// IS_DEBUG 0: disables printing statements for debugging
// set to true by default, but can redefine it to false when compiling
#ifndef IS_DEBUG
#define IS_DEBUG 1
#endif

// log statements will be printed if we're debugging
// otherwise does nothing
#if IS_DEBUG

#include <stdio.h>
#define log printf

#else

static inline void do_nothing(const char *format, ...) 
{
    (void) format;
}

#define log do_nothing

#endif



#include "log.c.generated.h"
#endif
