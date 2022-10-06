#ifndef INTERPRETER
#define INTERPRETER

#include "datatype.h"

struct VarEntry
{
    char *var_name;
    struct Literal *value;
};

// need to turn this into a proper hash table eventually, for now just an fixed-sized array
struct Env
{
    unsigned int size;
    struct VarEntry **var_entries;
};

enum ErrorCode
{
    TYPE_ERROR,
    REDEFINE_EXISTING_VAR,
    ASSIGNMENT_BEFORE_DECLARATION,
    UNDECLARED_VARIABLE,
    NO_ERROR
};

struct Interpreter
{
    enum ErrorCode error_code;
    // char *error;
    // struct Statements *stmts;
    struct Env *env;
};

#include "datatype.h"
#include "interpreter.c.generated.h"
#endif
