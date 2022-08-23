#include "parser.h"

struct Parser *parse(struct Lexer *lexer)
{
    struct Parser *parser;
    int max_errors = 10; 
    parser = malloc(sizeof(parser));
    parser->errors = calloc(max_errors, sizeof(char*));

    // enum TokenType t;
    // for (t = lexer->type; t != END_OF_STREAM; lexer = lexer->next, t = lexer->type)
    // {
    //     switch (t)
    //     {
    //         // case END_OF_STREAM,
    //         // case END_OF_LINE,
    //         // case IF_TOK,
    //         // case ELSE_TOK,
    //         // case ELSEIF,
    //         // case END,
    //         // case WHILE,
    //         // case NEXT,
    //         // case TRUE,
    //         // case FALSE,
    //         // case OPEN_PAREN,
    //         // case CLOSE_PAREN,
    //         // case COMMA,
    //         // case NOT_TOK,
    //         // case AND_TOK,
    //         // case OR_TOK,
    //         // case EQ_TOK,
    //         // case // DOUBLE_EQ,
    //         // case NEQ_TOK,
    //         // case LEQ_TOK,
    //         // case GEQ_TOK,
    //         // case GREATER_TOK,
    //         // case LESS_TOK,
    //         // case PLUS,
    //         // case MINUS,
    //         // case STAR,
    //         // case SLASH
    //     }
    // }
    // case IF:
    // case LOOP:
    // case DECLARATION:
    // case ASSIGNMENT:
    // case PRINT:
}

// Groups left to right for now, because it's easier
// Add a more sensible order of operations later
char *parse_expression(struct Expression **expression, struct Lexer *lexer)
{
    bool unary_ok, value_ok, binary_ok;
    int paren_count;

    unary_ok = true;
    value_ok = true;
    binary_ok = false;

    paren_count = 0;

    enum TokenType t;
    for (t = lexer->type; is_operator_token(t); lexer = lexer->next, t = lexer->type)
    {
    }
    return NULL;
}
