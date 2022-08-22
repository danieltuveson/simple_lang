#ifndef PARSER
#define PARSER

#include "stdincludes.h"
#include "lexer.h"

enum LiteralType
{
    NONE,
    BOOLEAN,
    INTEGER,
    STRING,
    VARIABLE
    // ARRAY
};

struct Literal
{
    enum LiteralType type;
    union
    {
        bool boolean;
        int integer;
        char *string;
        char *variable;
        // struct Expression *array;
    };
};

enum Operation
{
    NOT,
    AND,
    OR,
    EQ,
    GEQ,
    LEQ,
    GREATER,
    LESS,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    CONCAT
};

enum ExpressionType
{
    EXPRESSION,
    LITERAL
};

union ExpressionContent
{
    struct Expression *expression;
    struct Literal *literal;
};

// Not all of these need to be populated, e.g. a single literal won't have
// an operation and a NOT operation will only take one content as input.
struct Expression
{
    enum ExpressionType Operation;
    enum ExpressionType type1;
    union ExpressionContent *content1;
    enum ExpressionType type2;
    union ExpressionContent *content2;
};

enum StatementType
{
    IF,
    // IF_ELSE,
    LOOP,
    DECLARATION,
    ASSIGNMENT,
    PRINT
};

struct If
{
    struct Expression *condition;
    struct Statement **statements;
};

struct Loop
{
    struct Expression *condition;
    struct Statement **statements;
};

typedef char *Variable;
typedef Variable *Declaration;

struct Assignment
{
    Variable variable;
    struct Expression *value;
};

struct Statement
{
    enum StatementType type;
    union
    {
        struct If *if_statement;
        struct Loop *loop;
        struct Declaration *declaration;
        struct Assignment *assignment;
        char *var_to_print;
    };
};

struct Statements
{
    int size;
    int capacity;
    struct Statement **statements;
};

struct Parser
{
    char **errors;
    // struct Lexer *lexer;
    struct Statements *statements;
};

#include "parser.c.generated.h"
#endif