#ifndef PARSER
#define PARSER

#include "stdincludes.h"
#include "lexer.h"
#include "datatype.h"

enum ParseType
{
    PARSE_ERROR,
    END_OF_EXPRESSION,
    PARSED_EXPRESSION
};

struct Parser
{
    enum ParseType type;
    struct Lexer *lexer;
    // Going to remove union since partial expression could be 
    // useful for error messages?
    // union
    // {
    char *error;
    struct Expr *expr;
    // };
};

struct ParseStmts
{
    enum ParseType type;
    struct Lexer *lexer;
    char *error;
    struct Statements *stmts;
};

static inline int get_operator_precedence(enum SimpleToken simple_token)
{
    if (simple_token == OPEN_PAREN || simple_token == CLOSE_PAREN)
    {
        return 1;
    }
    else if (simple_token == STAR || simple_token == SLASH)
    {
        return 2;
    }
    else if (simple_token == PLUS || simple_token == MINUS)
    {
        return 3;
    }
    else if (simple_token == LEQ_TOK || simple_token == GEQ_TOK || 
            simple_token == GREATER_TOK || simple_token == LESS_TOK)
    {
        return 4;
    }
    else if (simple_token == DOUBLE_EQ || simple_token == NEQ_TOK)
    {
        return 5;
    }
    else if (simple_token == AND_TOK)
    {
        return 6;
    }
    else if (simple_token == OR_TOK)
    {
        return 7;
    }
    assert(false);
    return -1;
}

static inline enum BinaryOp token_to_op(enum SimpleToken st)
{
    switch (st)
    {
        case AND_TOK:
            return AND;
        case OR_TOK:
            return OR;
        case DOUBLE_EQ:
            return EQ;
        case NEQ_TOK:
            return NEQ;
        case GEQ_TOK:
            return GEQ;
        case LEQ_TOK:
            return LEQ;
        case GREATER_TOK:
            return GREATER;
        case LESS_TOK:
            return LESS;
        case PLUS:
            return ADD;
        case MINUS:
            return SUBTRACT;
        case STAR:
            return MULTIPLY;
        case SLASH:
            return DIVIDE;
        default:
            assert(false);
    }
};



#include "parser.c.generated.h"
#endif
