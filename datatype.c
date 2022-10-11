#include "datatype.h"

struct Statements *new_stmts(void)
{
    struct Statements *stmts;
    stmts = malloc(sizeof(struct Statements));
    stmts->size = 0;
    stmts->capacity = 5; // this is arbitrary, might change in the future
    stmts->stmts = calloc(stmts->capacity, sizeof(struct Statement)); 
    return stmts;
}

void add_stmt(struct Statements **stmts_ptr, struct Statement *stmt)
{
    struct Statements *stmts;
    stmts = *stmts_ptr;
    if (stmts->size == stmts->capacity)
        grow_stmts(stmts_ptr);
    stmts->stmts[(*stmts_ptr)->size] = stmt;
    stmts->size++;
}

// probably never need to shrink
// once we have the expressions we'll either evaluate or free them
void grow_stmts(struct Statements **stmts_ptr)
{
    struct Statements *stmts, *old_stmts;

    stmts = new_stmts();
    old_stmts = *stmts_ptr;

    stmts->capacity = 2 * old_stmts->capacity;
    stmts->size = old_stmts->size;
    stmts->stmts = calloc(stmts->capacity, sizeof(struct Statement));

    for (int i = 0; i < stmts->size; i++)
    {
        stmts->stmts[i] = old_stmts->stmts[i];
    }

    free(old_stmts->stmts);
    free(old_stmts);
    *stmts_ptr = stmts;
}

struct Statement *new_while_loop(struct Expr *expr, struct Statements *stmts)
{
    struct While *while_loop;
    while_loop = malloc(sizeof(struct While));
    while_loop->condition = expr;
    while_loop->stmts = stmts;

    struct Statement *stmt;
    stmt = malloc(sizeof(struct Statement));
    stmt->type = WHILE_LOOP;
    stmt->while_loop = while_loop;
    return stmt;
}

struct Statement *new_assignment(char *var_name, struct Expr *expr)
{
    struct Assignment *assignment;
    assignment = malloc(sizeof(struct Assignment));
    assignment->variable = var_name;
    assignment->value = expr;

    struct Statement *stmt;
    stmt = malloc(sizeof(struct Statement));
    stmt->type = ASSIGNMENT;
    stmt->assignment = assignment;
    return stmt;
}

struct Statement *new_declaration(unsigned int length, char **names)
{
    struct Declaration *decl = malloc(sizeof(struct Declaration));
    decl->length = length;
    decl->names = names;

    struct Statement *stmt;
    stmt = malloc(sizeof(struct Statement));
    stmt->type = DECLARATION;
    stmt->declaration = decl;
    return stmt;
}

struct Expr *new_literal(struct Literal *lit)
{
    struct Expr *expr;
    expr = malloc(sizeof(struct Expr));
    expr->type = LITERAL;
    expr->literal = lit;
    return expr;
}

struct Literal *new_lbool(enum SimpleToken st)
{
    struct Literal *lit;
    lit = malloc(sizeof(struct Literal));
    lit->type = BOOLEAN;
    lit->boolean = st == TRUE ? true : false;
    return lit;
}

struct Literal *new_lnumber(int num)
{
    struct Literal *lit;
    lit = malloc(sizeof(struct Literal));
    lit->type = NUMBER;
    lit->number = num;
    return lit;
}

struct Literal *new_lstring(char *str)
{
    struct Literal *lit;
    lit = malloc(sizeof(struct Literal));
    lit->type = STRING;
    lit->string = str;
    return lit;
}

struct Literal *new_lvar(char *var)
{
    struct Literal *lit;
    lit = malloc(sizeof(struct Literal));
    lit->type = VARIABLE;
    lit->variable = var;
    return lit;
}


// IF
// WHILE_LOOP
// DECLARATION
// ASSIGNMENT
// FUNCTION_CALL_STMT
