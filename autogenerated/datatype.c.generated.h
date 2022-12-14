struct Statements *new_stmts(void);
void add_stmt(struct Statements **stmts_ptr, struct Statement *stmt);
void grow_stmts(struct Statements **stmts_ptr);
struct Statement *new_while_loop(struct Expr *expr, struct Statements *stmts);
struct Statement *new_assignment(char *var_name, struct Expr *expr);
struct Statement *new_declaration(unsigned int length, char **names);
struct Expr *new_literal(struct Literal *lit);
struct Literal *new_lbool(enum SimpleToken st);
struct Literal *new_lnumber(int num);
struct Literal *new_lstring(char *str);
struct Literal *new_lvar(char *var);
struct Expr *new_bin_expr(enum BinaryOp binop, struct Expr *left_expr, struct Expr *right_expr);
