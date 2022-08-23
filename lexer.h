#ifndef LEXER
#define LEXER

#include "stdincludes.h"

enum SimpleToken
{
    END_OF_STREAM,
    END_OF_LINE,
    IF_TOK,
    ELSE_TOK,
    ELSEIF,
    END,
    WHILE,
    NEXT,
    TRUE,
    FALSE,
    OPEN_PAREN,
    CLOSE_PAREN,
    COMMA,
    NOT_TOK,
    AND_TOK,
    OR_TOK,
    EQ_TOK,
    // DOUBLE_EQ,
    NEQ_TOK,
    LEQ_TOK,
    GEQ_TOK,
    GREATER_TOK,
    LESS_TOK,
    PLUS,
    MINUS,
    STAR,
    SLASH
};

enum TokenType
{
    SIMPLE_TOKEN,
    NUMBER_TOKEN,
    STRING_TOKEN,
    COMMENT_TOKEN,
    VARIABLE_TOKEN
};

struct Lexer
{
    unsigned long line_number;
    unsigned long column_number;
    enum TokenType type;
    union
    {
        enum SimpleToken simple_token;
        int number;
        char *string;
        char *comment;
        char *variable;
    };
    struct Lexer *next;
};

static inline bool is_delimiter(char c)
{
    return ((c == '\0') || (c == ' ') || (c == '\n') || (c == ')') || (c == '(') || (c == ',') || (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '=') || (c == '<') || (c == '>'));
}
static inline char *print_simple_token(enum SimpleToken token)
{
    switch (token)
    {
    case END_OF_STREAM:
        return "END_OF_STREAM";
    case END_OF_LINE:
        return "END_OF_LINE";
    case IF_TOK:
        return "IF_TOK";
    case ELSE_TOK:
        return "ELSE_TOK";
    case ELSEIF:
        return "ELSEIF";
    case END:
        return "END";
    case WHILE:
        return "WHILE";
    case NEXT:
        return "NEXT";
    case TRUE:
        return "TRUE";
    case FALSE:
        return "FALSE";
    case OPEN_PAREN:
        return "OPEN_PAREN";
    case CLOSE_PAREN:
        return "CLOSE_PAREN";
    case COMMA:
        return "COMMA";
    case NOT_TOK:
        return "NOT_TOK";
    case AND_TOK:
        return "AND_TOK";
    case OR_TOK:
        return "OR_TOK";
    case EQ_TOK:
        return "EQ_TOK";
    // case DOUBLE_EQ:
    //     return "DOUBLE_EQ";
    case NEQ_TOK:
        return "NEQ_TOK";
    case LEQ_TOK:
        return "LEQ_TOK";
    case GEQ_TOK:
        return "GEQ_TOK";
    case GREATER_TOK:
        return "GREATER_TOK";
    case LESS_TOK:
        return "LESS_TOK";
    case PLUS:
        return "PLUS";
    case MINUS:
        return "MINUS";
    case STAR:
        return "STAR";
    case SLASH:
        return "SLASH";
    }
}

static inline bool is_operator_token(enum SimpleToken simple_token)
{
    return (simple_token == OPEN_PAREN || 
    simple_token == CLOSE_PAREN || 
    simple_token == NOT_TOK || 
    simple_token == AND_TOK || 
    simple_token == OR_TOK || 
    simple_token == EQ_TOK || 
    simple_token == NEQ_TOK || 
    simple_token == LEQ_TOK || 
    simple_token == GEQ_TOK || 
    simple_token == GREATER_TOK || 
    simple_token == LESS_TOK || 
    simple_token == PLUS || 
    simple_token == MINUS || 
    simple_token == STAR || 
    simple_token == SLASH);
}

#include "lexer.c.generated.h"
#endif
