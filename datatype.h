#ifndef DATATYPE
#define DATATYPE

#include "stdincludes.h"

enum LiteralType
{
    NONE,
    BOOLEAN,
    NUMBER,
    STRING,
    VARIABLE,
    FUNCTION_CALL
        // ARRAY
};

struct FunctionCall
{
    char *name;
    char **args;
};

struct Literal
{
    enum LiteralType type;
    union
    {
        bool boolean;
        int number;
        char *string;
        char *variable;
        struct FunctionCall *function_call;
        // struct Expr *array;
    };
};

enum UnaryOp
{
    NOT,
    NEGATE
};

enum BinaryOp
{
    AND,
    OR,
    EQ,
    NEQ,
    GEQ,
    LEQ,
    GREATER,
    LESS,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
};

enum ExprType
{
    UNARY_EXPRESSION,
    BINARY_EXPRESSION,
    LITERAL
};

struct UnaryExpr
{
    enum UnaryOp operation;
    struct Expr *expr;
};

struct BinaryExpr
{
    enum BinaryOp operation;
    struct Expr *expr1;
    struct Expr *expr2;
};

struct Expr
{
    enum ExprType type;
    union
    {
        struct UnaryExpr *unary_expr;
        struct BinaryExpr *binary_expr;
        struct Literal *literal;
    };
};

struct If
{
    struct Expr *condition;
    struct Statements *stmts;
};

struct Loop
{
    struct Expr *condition;
    struct Statements *stmts;
};

// typedef char *Variable;

struct Assignment
{
    char *variable;
    struct Expr *value;
};

enum StatementType
{
    IF,
    // IF_ELSE,
    LOOP,
    DECLARATION,
    ASSIGNMENT,
    FUNCTION_CALL_STMT
    // FUNCTION_DEFINITION
};

struct Statement
{
    enum StatementType type;
    union
    {
        struct If *if_stmt;
        struct Loop *loop;
        char **declaration;
        struct Assignment *assignment;
        struct FunctionCall *function_call;
    };
};

struct Statements
{
    int size;
    int capacity;
    struct Statement **stmts;
};


#include "datatype.c.generated.h"
#endif
