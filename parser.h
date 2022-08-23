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
        char *variable_name;
        // struct Expression *array;
    };
};

enum UnaryOperation
{
    NOT,
    NEGATE
};

enum BinaryOperation
{
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
    UNARY_EXPRESSION,
    BINARY_EXPRESSION,
    LITERAL
};

union ExpressionContent
{
    struct Expression *expression;
    struct Literal *literal;
};

struct UnaryExpression
{
    enum UnaryOperation operation;
    enum ExpressionType type;
    union ExpressionContent *content;
};

struct BinaryExpression
{
    enum BinaryOperation operation;
    enum ExpressionType type1;
    union ExpressionContent *content1;
    enum ExpressionType type2;
    union ExpressionContent *content2;
};

// Not all of these need to be populated, e.g. a single literal won't have
// an operation and a NOT operation will only take one content as input.
struct Expression
{
    enum ExpressionType type;
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

enum StatementType
{
    IF,
    // IF_ELSE,
    LOOP,
    DECLARATION,
    ASSIGNMENT,
    PRINT
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
