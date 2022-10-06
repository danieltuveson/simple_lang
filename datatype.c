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
    struct While *while_loop = malloc(sizeof(struct If));
    while_loop->condition = expr;
    while_loop->stmts = stmts;

    struct Statement *stmt;
    stmt = malloc(sizeof(struct Statement));
    stmt->type = WHILE_LOOP;
    stmt->while_loop = while_loop;
    return stmt;
}

// IF
// WHILE_LOOP
// DECLARATION
// ASSIGNMENT
// FUNCTION_CALL_STMT
