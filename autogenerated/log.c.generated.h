char *st_to_str(enum SimpleToken token);
void print_token(struct Lexer *lexer);
void print_lexer(struct Lexer *lexer);
void print_literal(struct Literal *literal);
void print_uop(enum UnaryOp uop);
void print_binop(enum BinaryOp binop);
void print_expr(struct Expr *expr);
void print_stmt(int padding, struct Statement *stmt);
void print_statements(struct Statements *stmts);
void print_padding(int padding);
void print_assignment(struct Assignment *assignment);
char *err_code_to_str(enum ErrorCode err);
void print_env(struct Env *env);
void print_interp(struct Interpreter *interp);
