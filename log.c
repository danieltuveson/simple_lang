#include "log.h"
// Various printers used for debugging

// Lexer
// prints the string representation of a simple token
char *st_to_str(enum SimpleToken token)
{
    switch (token)
    {
        case END_OF_STREAM:
            return "END_OF_STREAM";
        case END_OF_LINE:
            return "END_OF_LINE";
        case DIM:
            return "DIM";
        case IF_TOK:
            return "IF_TOK";
        case ELSE_TOK:
            return "ELSE_TOK";
        case ELSEIF:
            return "ELSEIF";
        case THEN:
            return "THEN";
        case END:
            return "END";
        case WHILE_TOK:
            return "WHILE_TOK";
        case NEXT:
            return "NEXT";
        case TRUE:
            return "TRUE";
        case FALSE:
            return "FALSE";
        case OPEN_PAREN:
            return "OPEN_PAREN";
        case CLOSE_PAREN:
            return "CLOSE_PAREN";
        case COMMA:
            return "COMMA";
        case NOT_TOK:
            return "NOT_TOK";
        case AND_TOK:
            return "AND_TOK";
        case OR_TOK:
            return "OR_TOK";
        case EQ_TOK:
            return "EQ_TOK";
        case DOUBLE_EQ:
            return "DOUBLE_EQ";
        case NEQ_TOK:
            return "NEQ_TOK";
        case LEQ_TOK:
            return "LEQ_TOK";
        case GEQ_TOK:
            return "GEQ_TOK";
        case GREATER_TOK:
            return "GREATER_TOK";
        case LESS_TOK:
            return "LESS_TOK";
        case PLUS:
            return "PLUS";
        case MINUS:
            return "MINUS";
        case STAR:
            return "STAR";
        case SLASH:
            return "SLASH";
    }
}

void print_token(struct Lexer *lexer)
{
    log("line %lu,col %lu: (", lexer->line_number, lexer->column_number);
    switch (lexer->type)
    {
        case SIMPLE_TOKEN:
            log("SIMPLE_TOKEN,");
            log("%s", st_to_str(lexer->simple_token));
            break;
        case NUMBER_TOKEN:
            log("NUMBER_TOKEN,");
            log("%d", lexer->number);
            break;
        case STRING_TOKEN:
            log("STRING_TOKEN,");
            log("\"%s\"", lexer->string);
            break;
        case VARIABLE_TOKEN:
            log("VARIABLE_TOKEN,");
            log("%s", lexer->variable);
            break;
        case COMMENT_TOKEN:
            log("COMMENT_TOKEN,");
            log("%s", lexer->comment);
            break;
        default:
            log("Unknown type");
            break;
    }
    log(")");
}

void print_lexer(struct Lexer *lexer)
{
    if (lexer == NULL)
        return;
    while (!(lexer->type == SIMPLE_TOKEN && lexer->simple_token == END_OF_STREAM))
    {
        print_token(lexer);
        log("\n");
        lexer = lexer->next;
    }
    print_token(lexer);
    log("\n");
}

// Expressions and Statements
void print_literal(struct Literal *literal)
{
    char **args;
    switch (literal->type)
    {
        case NONE:
            log("None");
            break;
        case BOOLEAN:
            if (literal->boolean == true)
                log("true");
            else 
                log("false");
            break;
        case NUMBER:
            log("%d", literal->number);
            break;
        case STRING:
            log("\"%s\"", literal->string);
            break;
        case VARIABLE:
            log("%s", literal->variable);
            break;
        case FUNCTION_CALL:
            args = literal->function_call->args;
            log("%s(%s", literal->function_call->name, *args);
            args++;
            while (args != NULL)
            {
                log(", %s\n", *args);
                args++;
            }
            log(")");
            break;
    }
}

void print_uop(enum UnaryOp uop)
{
    switch(uop)
    {
        case NOT:
            log("not");
            break;
        case NEGATE:
            log("-");
            break;
    }
};

void print_binop(enum BinaryOp binop)
{
    switch(binop)
    {
        case AND:
            log("and");
            break;
        case OR:
            log("or");
            break;
        case EQ:
            log("==");
            break;
        case NEQ:
            log("<>");
            break;
        case GEQ:
            log(">=");
            break;
        case LEQ:
            log("<=");
            break;
        case GREATER:
            log(">");
            break;
        case LESS:
            log("<");
            break;
        case ADD:
            log("+");
            break;
        case SUBTRACT:
            log("-");
            break;
        case MULTIPLY:
            log("*");
            break;
        case DIVIDE:
            log("/");
            break;
    }
};

void print_expr(struct Expr *expr)
{
    switch (expr->type)
    {
        case UNARY_EXPRESSION:
            log("(");
            log(" ");
            print_uop(expr->unary_expr->op);
            print_expr(expr->unary_expr->expr);
            log(")");
            break;
        case BINARY_EXPRESSION:
            log("(");
            print_binop(expr->binary_expr->op);
            log(" ");
            print_expr(expr->binary_expr->expr1);
            log(" ");
            print_expr(expr->binary_expr->expr2);
            log(")");
            break;
        case LITERAL:
            print_literal(expr->literal);
            break;
    }
}

void print_stmt(int padding, struct Statement *stmt)
{
    log("type: ");
    switch(stmt->type)
    {
        case IF:
            log("if\n");
            print_padding(padding);
            log("condition: ");
            print_expr(stmt->if_stmt->condition);
            log("\n");
            print_statements(stmt->if_stmt->stmts);
            break;
        case WHILE_LOOP:
            log("while\n");
            print_padding(padding);
            log("condition: ");
            print_expr(stmt->while_loop->condition);
            log("\n");
            print_statements(stmt->while_loop->stmts);
            break;
        case DECLARATION:
            log("dim ");
            for (unsigned int i = 0; i < stmt->declaration->length; i++)
            {
                log("%s", stmt->declaration->names[i]);
                if (i + 1 < stmt->declaration->length)
                    log(", ");
            }
            log("\n");
            break;
        case ASSIGNMENT:
            log("assignment\n");
            print_padding(padding);
            log("content: ");
            print_assignment(stmt->assignment);
            break;
        case FUNCTION_CALL_STMT:
            log("printer not implemented\n");
            break;
        default:
            log("unable to print input\n");
            break;
    }
}

void print_statements(struct Statements *stmts)
{
    static int tab_fact = 0;
    int tabwidth = 4;
    if (stmts == NULL)
    {
        print_padding(tab_fact * tabwidth);
        log("(no statements)\n");
        return;
    }
    print_padding(tab_fact * tabwidth);
    log("statements (size: %d, capacity: %d) [\n",  stmts->size, stmts->capacity);
    tab_fact++;
    struct Statement *stmt;
    for (int i = 0; i < stmts->size; i++)
    {
        print_padding(tab_fact * tabwidth);
        log("statement %d: {\n", i + 1);
        tab_fact++;
        print_padding(tab_fact * tabwidth);

        stmt = stmts->stmts[i];
        print_stmt(tab_fact * tabwidth, stmt);

        tab_fact--;
        print_padding(tab_fact * tabwidth);
        log("}\n");
    }
    tab_fact--;
    print_padding(tab_fact * tabwidth);
    log("]\n");

    // struct Statement
    // {
    //     enum StatementType type;
    //     union
    //     {
    //         struct If *if_statement;
    //         struct Loop *loop;
    //         struct Declaration *declaration;
    //         struct Assignment *assignment;
    //         struct FunctionCall *function_call;
    //     };
    // };
}

void print_padding(int padding)
{
    for (int i = 0; i < padding; i++)
        log(" ");
}

void print_assignment(struct Assignment *assignment)
{
    log("%s", assignment->variable);
    log(" = ");
    print_expr(assignment->value);
    log("\n");
}

char *err_code_to_str(enum ErrorCode err)
{
    switch(err)
    {
        case TYPE_ERROR:
            return "TYPE_ERROR";
        case REDEFINE_EXISTING_VAR:
            return "REDEFINE_EXISTING_VAR";
        case NO_ERROR:
            return "NO_ERROR";
        case ASSIGNMENT_BEFORE_DECLARATION:
             return "ASSIGNMENT_BEFORE_DECLARATION";
        case UNDECLARED_VARIABLE:
             return "UNDECLARED_VARIABLE";
        default:
            return "unknown error code";
    }
}

void print_env(struct Env *env)
{
    struct VarEntry *var_entry = NULL;
    if (env->size == 0) 
    {
        log("env: (empty)\n");
        return;
    }
    log("env: {\n");
    for (unsigned int i = 0; i < env->size; i++)
    {
        print_padding(4);
        var_entry = env->var_entries[i];
        log("%s = ", var_entry->var_name);
        print_literal(var_entry->value);
        log("\n");
    }
    log("}\n");
}

void print_interp(struct Interpreter *interp)
{
   log("error code: %s\n", err_code_to_str(interp->error_code));
   print_env(interp->env);
}
