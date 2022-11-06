#include "tests.h"
#include "vect.h"
#include "datatype.h"

// Helper functions for quickly loading in files to test
// struct Lexer *lex_from_file(char *filename)
// {
//     char *content;
//     struct Lexer *lexer;
// 
//     content = read_file(filename);
//     assert(content != NULL);
// 
//     lexer = malloc(sizeof(struct Lexer));
//     assert(tokens(lexer, content) == NULL);
//     return lexer;
// }
// 
// struct Parser *parse_from_file(char *filename)
// {
//     struct Parser *parser;
//     parser = parse_expr(lex_from_file(filename));
//     assert(parser->type != PARSE_ERROR)
//     assert(parser->type != END_OF_EXPRESSION)
//     return parser;
// }


// Tests
void test_vect()
{
    struct Statement *stmt = new_assignment("panda", new_literal(new_lbool(true)));

    printf("%lu\n", sizeof(struct Statements));
    printf("%lu\n", sizeof(struct Vect));

    struct Statements *stmts = new_stmts();
    stmts->stmts[0] = stmt;
    printf("%lu\n", sizeof(stmts->stmts));

    struct Vect *vect = new_vect();
    vect->array[0] = stmt;

    printf("%lu\n", sizeof(vect->array[0]));
    printf("%lu\n", sizeof(int));
    assert(stmts->stmts[0] == vect->array[0]);
}

void test_lexer()
{
}

void test_expression_parser()
{
}
