struct Statements *new_stmts(void);
void add_stmt(struct Statements **stmts_ptr, struct Statement *stmt);
void grow_stmts(struct Statements **stmts_ptr);
struct Statement *new_while_loop(struct Expr *expr, struct Statements *stmts);
