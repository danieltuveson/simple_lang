#ifndef LEXER
#define LEXER

#include "stdincludes.h"

enum SimpleToken
{
    END_OF_STREAM,
    END_OF_LINE,
    DIM,
    IF_TOK,
    ELSE_TOK,
    ELSEIF,
    THEN,
    END,
    WHILE_TOK,
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
    DOUBLE_EQ,
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

static inline bool is_operator_token(enum SimpleToken simple_token)
{
    return (simple_token == OPEN_PAREN || 
            simple_token == CLOSE_PAREN || 
            simple_token == NOT_TOK || 
            simple_token == AND_TOK || 
            simple_token == OR_TOK || 
            simple_token == DOUBLE_EQ || 
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
