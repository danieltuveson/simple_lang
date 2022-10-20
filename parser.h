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

// enum ParserType
// {
//     P_STATEMENTS,
//     P_EXPRESSION,
//     P_LITERAL
// };

struct Parser
{
    enum ParseType type;
    struct Lexer *lexer;
    char *error;

    struct Expr *expr;
    // Maybe will just shove everything in one parser to make my life simpler?
    // enum ParserType;
    // struct Statements *stmts;
};

struct ParseStmts
{
    enum ParseType type;
    struct Lexer *lexer;
    char *error;
    struct Statements *stmts;
};

static inline int get_operator_precedence(enum SimpleToken st)
{
    if (st == OPEN_PAREN || st == CLOSE_PAREN)
    {
        return 7;
    }
    else if (st == STAR || st == SLASH)
    {
        return 6;
    }
    else if (st == PLUS || st == MINUS)
    {
        return 5;
    }
    else if (st == LEQ_TOK || st == GEQ_TOK || 
            st == GREATER_TOK || st == LESS_TOK)
    {
        return 4;
    }
    else if (st == DOUBLE_EQ || st == NEQ_TOK)
    {
        return 3;
    }
    else if (st == AND_TOK)
    {
        return 2;
    }
    else if (st == OR_TOK)
    {
        return 1;
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
