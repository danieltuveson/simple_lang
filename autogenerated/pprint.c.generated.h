char *print_simple_token(enum SimpleToken token);
void print_lexer(struct Lexer *lexer);
void print_literal(struct Literal *literal);
void print_unary_operation(enum UnaryOperation unary_operation);
void print_binary_operation(enum BinaryOperation binary_operation);
void print_expression(struct Expression *expression);
void print_statements(struct Statements *stmts);
void print_padding(int padding);
void print_assignment(struct Assignment *assignment);
