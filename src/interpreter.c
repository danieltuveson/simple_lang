#include "interpreter.h"
#include "log.h"

enum ErrorCode interpret(struct Statements *stmts)
{
    struct Interpreter *interp = malloc(sizeof(struct Interpreter));
    interp->error_code = NO_ERROR;
    interp->env = new_environment();
    return exec(interp, stmts);
}

enum ErrorCode exec(struct Interpreter *interp, struct Statements *stmts)
{
    struct Statement *stmt = NULL;
    for (int i = 0; i < stmts->size; i++)
    {
        print_interp(interp);
        stmt = stmts->stmts[i];
        switch (stmt->type)
        {
            case IF:
                exec_if(interp, stmt->if_stmt);
                break;
            case WHILE_LOOP:
                exec_while(interp, stmt->while_loop);
                break;
            case DECLARATION:
                exec_declaration(interp, stmt->declaration);
                break;
            case ASSIGNMENT:
                exec_assignment(interp, stmt->assignment);
                break;
            case FUNCTION_CALL_STMT:
                assert(false);
                break;
        }
        if (interp->error_code != NO_ERROR)
            break;
    }
    if (stmt == NULL)
    {
        print_interp(interp);
    }
    return interp->error_code;
}

enum ErrorCode exec_while(struct Interpreter *interp, struct While *while_loop)
{
    struct Literal *lit;

loop:
    lit = eval(interp, while_loop->condition);

    if (interp->error_code != NO_ERROR)
    {
        return interp->error_code;
    }
    else if (lit->type == BOOLEAN && lit->boolean == true)
    {
        exec(interp, while_loop->stmts);
        goto loop;
    }
    else if (lit->type != BOOLEAN && lit->type != NONE)
    {
        interp->error_code = TYPE_ERROR;
        return interp->error_code;
    }
    else 
    {
        return interp->error_code;
    }
}

enum ErrorCode exec_if(struct Interpreter *interp, struct If *if_stmt)
{
    struct Literal *lit = eval(interp, if_stmt->condition);
    if (interp->error_code != NO_ERROR)
    {
        return interp->error_code;
    }
    else if (lit->type == BOOLEAN && lit->boolean == true)
    {
        exec(interp, if_stmt->stmts);
    }
    else if (lit->type == NONE || lit->type == BOOLEAN)
    {
        // Do nothing, in the future handle else case 
    }
    else 
    {
        interp->error_code = TYPE_ERROR;
    }
    return interp->error_code;
}

void exec_assignment(struct Interpreter *interp, struct Assignment *assignment)
{
    if (!var_exists(interp->env, assignment->variable))
    {
        interp->error_code = ASSIGNMENT_BEFORE_DECLARATION;
        return;
    }
    struct Literal *lit = eval(interp, assignment->value);
    if (interp->error_code != NO_ERROR)
        return;
    add_var(interp->env, assignment->variable, lit);
    return;
}

void exec_declaration(struct Interpreter *interp, struct Declaration *decl)
{
    char *name = NULL;
    for (unsigned int i = 0; i < decl->length; i++)
    {
        name = decl->names[i];
        if (var_exists(interp->env, name))
        {
            interp->error_code = REDEFINE_EXISTING_VAR;
            return;
        }
        else
        {
            struct Literal *lit;
            lit = malloc(sizeof(struct Literal));
            lit->type = NONE;
            add_var(interp->env, name, lit);
        }
    }
}

#define MAX_ENV_SIZE 50

// need to turn this into a proper hash table eventually, for now just an fixed-sized array
struct Env *new_environment()
{
    struct Env *env = malloc(sizeof(struct Env));
    env->var_entries = calloc(MAX_ENV_SIZE, sizeof(struct VarEntry));
    env->size = 0;
    return env;
}

// -1 if doesn't exist
int var_entry_index(struct Env *env, char *var_name)
{
    for (unsigned int i = 0; i < env->size; i++)
    {
        if (strcmp(var_name, env->var_entries[i]->var_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

bool var_exists(struct Env *env, char *var_name)
{
    return (var_entry_index(env, var_name) != -1);
}

void add_var(struct Env *env, char *var_name, struct Literal *lit)
{
    if (env->size == MAX_ENV_SIZE)
    {
        assert(false);
    }
    int i = var_entry_index(env, var_name);
    if (i != -1)
    {
        env->var_entries[i]->value = lit;
    }
    else
    {
        struct VarEntry *var_entry = malloc(sizeof(struct VarEntry));
        var_entry->var_name = var_name;
        var_entry->value = lit;
        env->var_entries[env->size] = var_entry;
        env->size++;
    }
}

// assumes that variable exists
struct Literal *lookup_var(struct Env *env, char *var_name)
{
    for (unsigned int i = 0; i < env->size; i++)
    {
        if (strcmp(var_name, env->var_entries[i]->var_name) == 0)
        {
            return env->var_entries[i]->value;
        }
    }
    return NULL;
}

// correct ordering of operations happens in the parsing, so we just need to 
// recursively evaluate the arguments as seen in the Expr we are given
struct Literal *eval(struct Interpreter *interp, struct Expr *expr)
{
    // #define return_on_error(val, ex) 
    struct Literal *val;
    switch (expr->type)
    {
        case UNARY_EXPRESSION:
            eval_unary_expr(interp, expr->unary_expr);
            assert(false);
        case BINARY_EXPRESSION: 
            return eval_bin_exp(interp, expr->binary_expr);
        case LITERAL:
            switch(expr->literal->type)
            {
                case VARIABLE:
                    if (var_exists(interp->env, expr->literal->variable))
                    {
                        val = lookup_var(interp->env, expr->literal->variable);
                        return val;
                    }
                    else
                    {
                        interp->error_code = UNDECLARED_VARIABLE;
                        return NULL;
                    }
                default:
                    return expr->literal;
            }
    }
    assert(false);
    return NULL;
}


struct Literal *eval_bin_exp(struct Interpreter *interp, struct BinaryExpr *binary_expr)
{
    // need to adjust this a little bit to account for boolean operator 
    // short circuiting
    struct Literal *val1, *val2, *new_val;
    val1 = eval(interp, binary_expr->expr1); 
    if (interp->error_code != NO_ERROR)
    {
        return NULL;
    }
    else if (val1->type == NUMBER)
    {
        val2 = eval(interp, binary_expr->expr2); 
        if (interp->error_code != NO_ERROR)
        {
            return NULL;
        }
        else if (val2->type != NUMBER)
        {
            interp->error_code = TYPE_ERROR;
            return NULL;
        }
        new_val = malloc(sizeof(struct Literal));
        switch(binary_expr->op)
        {
            case ADD:
                new_val->type = NUMBER;
                new_val->number = val1->number + val2->number;
                return new_val;
            case SUBTRACT:
                new_val->type = NUMBER;
                new_val->number = val1->number - val2->number;
                return new_val;
            case MULTIPLY:
                new_val->type = NUMBER;
                new_val->number = val1->number * val2->number;
                return new_val;
            case DIVIDE:
                new_val->type = NUMBER;
                new_val->number = val1->number / val2->number;
                return new_val;
            case EQ:
                new_val->type = BOOLEAN;
                new_val->boolean = val1->number == val2->number;
                return new_val;
            case NEQ:
                new_val->type = BOOLEAN;
                new_val->boolean = val1->number != val2->number;
                return new_val;
            case GEQ:
                new_val->type = BOOLEAN;
                new_val->boolean = val1->number >= val2->number;
                return new_val;
            case LEQ:
                new_val->type = BOOLEAN;
                new_val->boolean = val1->number <= val2->number;
                return new_val;
            case GREATER:
                new_val->type = BOOLEAN;
                new_val->boolean = val1->number > val2->number;
                return new_val;
            case LESS:
                new_val->type = BOOLEAN;
                new_val->boolean = val1->number < val2->number;
                return new_val;
            default:
                assert(false);
        }
    }
    else if (val1->type == BOOLEAN || val1->type == NONE)
    {
        new_val = malloc(sizeof(struct Literal));
        switch(binary_expr->op)
        {
            case AND:
                if (val1->type == NONE)
                {
                    new_val->type = NONE;
                    return new_val;
                }
                else if (val1->boolean == false)
                {
                    new_val->type = BOOLEAN;
                    new_val->boolean = false;
                    return new_val;
                }
                val2 = eval(interp, binary_expr->expr2);
                if (interp->error_code != NO_ERROR)
                {
                    return NULL;
                }
                else if (val2->type == BOOLEAN)
                {
                    new_val->type = BOOLEAN;
                    new_val->boolean = val2->boolean;
                    return new_val;
                }
                else if (val2->type == NONE)
                {
                    new_val->type = NONE;
                    return new_val;
                }
                else 
                {
                    interp->error_code = TYPE_ERROR;
                    return NULL;
                }
            case OR:
                if (val1->boolean == true)
                {
                    new_val->type = BOOLEAN;
                    new_val->boolean = true;
                    return new_val;
                }
                val2 = eval(interp, binary_expr->expr2);
                if (interp->error_code != NO_ERROR)
                {
                    return NULL;
                }
                else if (val2->type == BOOLEAN)
                {
                    new_val->type = BOOLEAN;
                    new_val->boolean = val2->boolean;
                    return new_val;
                }
                else if (val2->type == NONE)
                {
                    new_val->type = NONE;
                    return new_val;
                }
                else 
                {
                    interp->error_code = TYPE_ERROR;
                    return NULL;
                }
            default:
                assert(false);
                break;
        }
    }
    else
    {
        interp->error_code = TYPE_ERROR;
        return NULL;
    }
    return new_val;
}

struct Literal *eval_unary_expr(struct Interpreter *interp, struct UnaryExpr *unary_expr)
{
    assert(false);
    struct Literal *val;
    switch (unary_expr->op)
    {
        case NOT:
            val = eval(interp, unary_expr->expr); 
            if (interp->error_code != NO_ERROR)
            {
                return NULL;
            }
            else if (val->type == NONE)
            {
                val->type = BOOLEAN;
                val->boolean = true;
                return val;
            }
            else if (val->type == BOOLEAN)
            {
                if (val->boolean == true)
                {
                    val->boolean = false;
                    return val;
                }
                else
                {
                    val->boolean = true;
                    return val;
                }
            }
            else
            {
                interp->error_code = TYPE_ERROR;
                return NULL;
            }
            break;
        case NEGATE:
            assert(false);
            break;
    }
    return NULL;
}
