#include "tests.h"

struct Lexer *lex_from_file(char *filename)
{
    char *content;
    struct Lexer *lexer;

    content = read_file(filename);
    assert(content != NULL);

    lexer = malloc(sizeof(struct Lexer));
    assert(tokens(lexer, content) == NULL);
    return lexer;
}

struct Parser *parse_from_file(char *filename)
{
    struct Parser *parser;
    parser = parse_expr(lex_from_file(filename));
    assert(parser->type != PARSE_ERROR)
    assert(parser->type != END_OF_EXPRESSION)
    return parser;
}

void test_lexer()
{
}

void test_expression_parser()
{
}
