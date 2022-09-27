#include "parser.h"
#include "error.h"
#include "log.h"

struct ParseStmts *new_parse_stmts(struct Lexer *lexer)
{
    struct ParseStmts *parser;
    parser = malloc(sizeof(struct ParseStmts));
    parser->error = NULL;
    parser->type  = PARSED_EXPRESSION;
    parser->stmts = new_stmts();
    parser->lexer = lexer;
    return parser;
}

struct ParseStmts *parse_all_stmts(struct Lexer *lexer)
{
    struct ParseStmts *parser = parse_stmts(lexer);
    return NULL;
}

struct ParseStmts *parse_stmts(struct Lexer *lexer)
{
    enum TokenType t;
    char *var;
    struct ParseStmts *parser;
    parser = new_parse_stmts(lexer);

    while (true)
    {
        log("line,col:%lu,%lu\n", parser->lexer->line_number, parser->lexer->column_number);
        t = parser->lexer->type; 
        switch (t)
        {
            case SIMPLE_TOKEN:
                switch (parser->lexer->simple_token)
                {
                    case IF_TOK:
                        parser->lexer = parser->lexer->next;
                        parser = parse_if(parser);
                        break;
                    case WHILE:
                        parse_loop(parser);
                        break;
                    case DIM:
                        parse_declarations(parser);
                        break;
                    case END_OF_STREAM:
                        return parser;
                    case END_OF_LINE:
                        // return parser;
                        break;
                    case END:
                        parser->type = END_OF_EXPRESSION;
                        return parser;
                    default:
                        parser->type = PARSE_ERROR;
                        parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "unexpected token for beginning of statement");
                        return parser;
                }
                break;
            case VARIABLE_TOKEN:
                var = parser->lexer->variable;
                parser->lexer = parser->lexer->next;
                if (parser->lexer->type == SIMPLE_TOKEN && parser->lexer->simple_token == OPEN_PAREN)
                {
                    // TODO add parser for function call
                    assert(false);
                }
                else if (parser->lexer->type == SIMPLE_TOKEN && parser->lexer->simple_token == EQ_TOK)
                {
                    parser->lexer = parser->lexer->next;
                    parser = parse_assignment(var, parser);
                    if (parser->type != PARSED_EXPRESSION)
                    {
                        return parser;
                    }
                }
                else
                {
                    parser->type = PARSE_ERROR;
                    parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "unexpected token, expected variable initialization or function call");
                    return parser;
                }
                break;
            default:
                parser->type = PARSE_ERROR;
                parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "unexpected token for beginning of statement");
                return parser;
        }
        if (parser->type == PARSE_ERROR || (parser->lexer->type == SIMPLE_TOKEN && parser->lexer->simple_token == END_OF_STREAM))
        {
            return parser;
        }
        parser->lexer = parser->lexer->next;
    }
    return parser;
}

struct ParseStmts *parse_assignment(char *var_name, struct ParseStmts *parser)
{
    struct Parser *exp_parser;
    exp_parser = parse_expr(parser->lexer);
    parser->lexer = exp_parser->lexer;
    if (exp_parser->type != PARSED_EXPRESSION)
    {
        parser->type = exp_parser->type;
        parser->error = exp_parser->error;
        return parser;
    }

    struct Assignment *assignment;
    assignment = malloc(sizeof(struct Assignment));
    assignment->variable = var_name;
    assignment->value = exp_parser->expr;

    struct Statement *stmt;
    stmt = malloc(sizeof(struct Statement));
    stmt->type = ASSIGNMENT;
    stmt->assignment = assignment;

    add_stmt(&(parser->stmts), stmt);
    parser->type = PARSED_EXPRESSION;
    return parser;
}

void parse_declarations(struct ParseStmts *parser)
{
    struct Parser *parsed_exp = parse_expr(parser->lexer);
    if (parsed_exp->type != PARSED_EXPRESSION)
    {
        parser->error = parsed_exp->error;
        parser->type = parsed_exp->type;
        return;
    }
    else
    {
    }
}

struct ParseStmts *parse_if(struct ParseStmts *parser)
{
    struct Parser *exp_parser;
    struct ParseStmts *body;
    exp_parser = parse_expr(parser->lexer);
    parser->lexer = exp_parser->lexer;

    // Parse valid expression
    if (exp_parser->type != PARSED_EXPRESSION)
    {
        parser->type = exp_parser->type;
        parser->error = exp_parser->error;
        return parser;
    }
    // parse "then"
    if (parser->lexer->type != SIMPLE_TOKEN || parser->lexer->simple_token != THEN)
    {
        parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "if statement must be terminated by 'then'");
        parser->type = PARSE_ERROR;
        return parser;
    }
    parser->lexer = parser->lexer->next;


    // parse newline
    if (parser->lexer->type != SIMPLE_TOKEN || parser->lexer->simple_token != END_OF_LINE)
    {
        parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "if statement must be terminated by newline");
        parser->type = PARSE_ERROR;
        return parser;
    }
    parser->lexer = parser->lexer->next;

    // parse statements
    body = parse_stmts(parser->lexer);
    parser->lexer = body->lexer;
    if (body->type == PARSE_ERROR)
    {
        parser->type = PARSE_ERROR;
        parser->error = body->error;
        return parser;
    }

    // parse end if 
    if (parser->lexer->type != SIMPLE_TOKEN || parser->lexer->simple_token != END)
    {
        parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "if statement must be terminated by 'end if'");
        parser->type = PARSE_ERROR;
        return parser;
    }
    parser->lexer = parser->lexer->next;
    if (parser->lexer->type != SIMPLE_TOKEN || parser->lexer->simple_token != IF_TOK)
    {
        parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "if statement must be terminated by 'end if'");
        parser->type = PARSE_ERROR;
        return parser;
    }
    parser->lexer = parser->lexer->next;

    struct If *if_stmt = malloc(sizeof(struct If));
    if_stmt->condition = exp_parser->expr;
    if_stmt->stmts = body->stmts;

    struct Statement *stmt;
    stmt = malloc(sizeof(struct Statement));
    stmt->type = IF;
    stmt->if_stmt = if_stmt;

    add_stmt(&(parser->stmts), stmt);
    parser->type = PARSED_EXPRESSION;

    return parser;
}

void parse_loop(struct ParseStmts *parser)
{
    return;
}


struct Parser *parse_expr(struct Lexer *lexer)
{
    struct Parser *parser;

    parser = malloc(sizeof(struct Parser));
    parser->lexer = lexer;
    parser->expr = NULL;

    parser = parse_literal(parser);

    if (parser->type != PARSED_EXPRESSION)
    {
        return parser;
    }
    parser->lexer = parser->lexer->next;
    return parse_exp_precedence(parser, 0);
}


// Operator precedence parser modified to accept parens + unary operators
struct Parser *parse_exp_precedence(struct Parser *parser, int min_precedence)
{
    enum TokenType t;
    enum SimpleToken st;
    struct Expr *expr = NULL;

    for (t = parser->lexer->type; t == SIMPLE_TOKEN; parser->lexer = parser->lexer->next)
    {
        st = parser->lexer->simple_token;
        if (!is_operator_token(st) || get_operator_precedence(st) < min_precedence)
        {
            return parser;
        }
        else if (st == CLOSE_PAREN)
        {
            parser->type = PARSE_ERROR;
            parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "no matching open paren");
            return parser;
        }
        else if (st == OPEN_PAREN)
        {
            // TODO
            parser->lexer = parser->lexer->next;
            parser = parse_literal(parser);
            if (parser->type != PARSED_EXPRESSION)
            {
                return parser;
            }

            parser = parse_exp_precedence(parser, min_precedence);
            if (parser->type != PARSED_EXPRESSION)
            {
                return parser;
            }
            if (parser->lexer->type != SIMPLE_TOKEN || parser->lexer->simple_token != CLOSE_PAREN)
            {
                parser->type = PARSE_ERROR;
                parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "expecting close paren");
            }
            parser->lexer = parser->lexer->next;
        }
        else // found operator token
        {
            parser->lexer = parser->lexer->next;

            // create new expression from token
            expr = malloc(sizeof(struct Expr));
            expr->type = BINARY_EXPRESSION;
            expr->binary_expr = malloc(sizeof(struct BinaryExpr));
            expr->binary_expr->operation = token_to_op(st);
            expr->binary_expr->expr1 = parser->expr;
            parser->expr = expr;
            parser = parse_literal(parser); // will populate expression
            if (parser->type != PARSED_EXPRESSION)
            {
                return parser;
            }
        }
    }
    return NULL;
}

struct Parser *parse_literal(struct Parser *parser)
{
    struct Expr *expr;
    struct Lexer *lexer;
    enum SimpleToken st;

    lexer = parser->lexer;
    expr = NULL;

    switch (lexer->type)
    {
        case SIMPLE_TOKEN:
            st = lexer->simple_token;
            if (st == TRUE || st == FALSE)
            {
                expr = malloc(sizeof(struct Expr));
                expr->type = LITERAL;
                expr->literal = malloc(sizeof(struct Literal));
                expr->literal->type = BOOLEAN;
                expr->literal->number = st == TRUE ? true : false;
                break;
            }
            else if (is_operator_token(st))
            {
                parser->type = PARSE_ERROR;
                parser->error = error(lexer->line_number, lexer->column_number, "operator not expected at this time");
                return parser;
            }
            else
            {
                parser->type = END_OF_EXPRESSION;
                return parser;
            }
            break;
        case NUMBER_TOKEN:
            expr = malloc(sizeof(struct Expr));
            expr->type = LITERAL;
            expr->literal = malloc(sizeof(struct Literal));
            expr->literal->type = NUMBER;
            expr->literal->number = lexer->number;
            break;
        case STRING_TOKEN:
            expr = malloc(sizeof(struct Expr));
            expr->type = LITERAL;
            expr->literal = malloc(sizeof(struct Literal));
            expr->literal->type = STRING;
            expr->literal->string = lexer->string;
            break;
        case COMMENT_TOKEN:
            parser->type = END_OF_EXPRESSION;
            return parser;
        case VARIABLE_TOKEN:
            // Handle function call option here eventually
            expr = malloc(sizeof(struct Expr));
            expr->type = LITERAL;
            expr->literal = malloc(sizeof(struct Literal));
            expr->literal->type = VARIABLE;
            expr->literal->variable = lexer->variable;
            break;
    }
    parser->type = PARSED_EXPRESSION;
    if (parser->expr == NULL)
    {
        parser->expr = expr;
    }
    else if (parser->expr->type == BINARY_EXPRESSION)
    {
        parser->expr->binary_expr->expr2 = expr;
    }
    else
    {
        assert(false);
    }
    // parser->lexer = lexer->next;
    return parser;
}

struct FunctionCall *parse_function_call(void)
{
    log("\nFATAL ERROR: parse_function_call not implemented\n");
    return NULL;
}

