#include "lexer.h"


void print_lexer(struct Lexer *lexer)
{
    if (lexer == NULL)
        return;
    printf("\nLexer\n{\n    line_number = %d\n    column_number = %d\n    ", lexer->line_number, lexer->column_number);
    printf("content = ");
    switch (lexer->type)
    {
    case SIMPLE_TOKEN:
        printf("%s", print_simple_token(lexer->simple_token));
        break;
    case NUMBER_TOKEN:
        printf("%d", lexer->number);
        break;
    case STRING_TOKEN:
        printf("\"%s\"", lexer->string);
        break;
    case VARIABLE_TOKEN:
        printf("%s", lexer->variable);
        break;
    default:
        printf("Unknown type");
        break;
    }
    printf("\n}\n");
    print_lexer(lexer->next);
}

// Break string into tokens
// If there's an error, stop lexing
char *tokens(struct Lexer *lexer, char *string)
{
    int num, i;
    char c;
    char *error;
    int line_number, column_number;
    line_number = 1;
    column_number = 1;
    error = calloc(200, sizeof(char));

    for (c = *string; c != '\0'; string++, c = *string)
    {
        // Can ignore whitespace and keep moving, doesn't actually affect token parsing
        if (c == ' ')
        {
            column_number++;
            continue;
        }

        // Sets line / column number for more helpful interpreter errors down the line
        lexer->column_number = column_number;
        lexer->line_number = line_number;

        // *Most* tokens will be simple tokens, so thats the default. 
        // Gets overridden further down if it's not a simple token
        lexer->type = SIMPLE_TOKEN;

        // one and two character tokens that don't need to be terminated by a space / newline
        if (c == '\n')
        {
            lexer->simple_token = END_OF_LINE;
            line_number++;
            column_number = 1;
        }
        else if (c == ')')
        {
            lexer->simple_token = CLOSE_PAREN;
            column_number++;
        }
        else if (c == '(')
        {
            lexer->simple_token = OPEN_PAREN;
            column_number++;
        }
        else if (c == ',')
        {
            lexer->simple_token = COMMA;
            column_number++;
        }
        else if (c == '+')
        {
            lexer->simple_token = PLUS;
            column_number++;
        }
        else if (c == '-')
        {
            lexer->simple_token = MINUS;
            column_number++;
        }
        else if (c == '*')
        {
            lexer->simple_token = STAR;
            column_number++;
        }
        else if (c == '/')
        {
            lexer->simple_token = SLASH;
            column_number++;
        }
        else if (c == '=')
        {
            string++;
            c = *string;
            column_number++;
            if (c == '=')
            {
                lexer->simple_token = DOUBLE_EQ;
                column_number++;
            }
            else
            {
                lexer->simple_token = EQ_TOK;
                string--;
            }
        }
        else if (c == '<')
        {
            string++;
            c = *string;
            column_number++;
            if (c == '>')
            {
                lexer->simple_token = NEQ_TOK;
                column_number++;
            }
            else if (c == '=')
            {
                lexer->simple_token = LEQ_TOK;
                column_number++;
            }
            else 
            {
                lexer->simple_token = LESS_TOK;
                string--;
            }

        }
        else if (c == '>')
        {
            string++;
            c = *string;
            column_number++;
            if (c == '=')
            {
                lexer->simple_token = GEQ_TOK;
                column_number++;
            }
            else 
            {
                lexer->simple_token = GREATER_TOK;
                string--;
            }
        }

        // Multi-character tokens

        // String
        // TODO: fix this
        else if (c == '"')
        {
            char prevc;
            char *str, *old_str;
            int capacity = 10;
            prevc = '\0';
            str = calloc(capacity, sizeof(char));
            i = 0;
            for (c = *string; c != '\0' && ((c != '"') || (prevc == '\\' && c == '"')); string++, c = *string)
            {
                if (i == (capacity - 1))
                {
                    capacity *= 2;
                    old_str = str;
                    str = calloc(capacity, sizeof(char));
                    strcpy(str, old_str);
                    free(old_str);
                }

                if (c == '\n')
                {
                    sprintf(error, "Error at line %d, column %d: newline in string literal", line_number, column_number);
                    return error;
                }
                else if (prevc == '\\') 
                {
                    switch (c)
                    {
                        case '\\':
                            *(str+i) = '\\';
                            break;
                        case '"':
                            *(str+i) = '"';
                            break;
                        case 'n':
                            *(str+i) = '\n';
                            break;
                        default:
                            sprintf(error, "Error at line %d, column %d: unknown escape sequence '\\%c'", line_number, column_number, c);
                            return error;
                    }
                    i++;
                }
                else if (c != '\\')
                {
                    *(str+i) = c;
                    i++;
                }
                // if c == '\\' then we keep going
                column_number++;
                prevc = c;
            }
            if (prevc == '\0' || prevc == '\n')
            {
                sprintf(error, "Error at line %d, column %d: unexpected end of string", line_number, column_number);
                return error;
            }
            lexer->type = STRING_TOKEN;
            lexer->string = str;
            printf("%c\n",c);
        }
        // Int
        else if (isdigit(c))
        {
            num = 0;
            i = 0;
            for (c = *string; !is_delimiter(c); string++, c = *string)
            {
                if (!isdigit(c))
                {
                    sprintf(error, "Error at line %d, column %d: alphanumeric may not begin with a digit", line_number, column_number);
                    return error;
                }
                i = c - '0';
                num *= 10;
                num += i;
                column_number++;
            }
            if (c != '\0') string--;
            lexer->type = NUMBER_TOKEN;
            lexer->number = num;
        }
        // Variable or Keyword
        else if (isalnum(c) || c == '_')
        {
            int max;
            char *word;
            for (max = 0, c = *string; !is_delimiter(c); string++, c = *string, max++)
            {
                if (!(isalnum(c) || c == '_'))
                {
                    sprintf(error, "Error at line %d, column %d: unknown character in token: %c", line_number, column_number, c);
                    return error;
                }
                column_number++;
            }
            string -= max;
            word = calloc(max + 1, sizeof(char));
            for (i = 0, c = *string; i < max; string++, c = *string, i++) 
            {
                *(word+i) = c;
            }
            if (c != '\0') string--;
            *(word + max) = '\0';

            // Check for keyword
            if (strcmp("if", word) == 0)
                lexer->simple_token = IF_TOK;
            else if (strcmp("elseif", word) == 0)
                lexer->simple_token = ELSEIF;
            else if (strcmp("else", word) == 0)
                lexer->simple_token = ELSE_TOK;
            else if (strcmp("end", word) == 0)
                lexer->simple_token = END;
            else if (strcmp("while", word) == 0)
                lexer->simple_token = WHILE;
            else if (strcmp("next", word) == 0)
                lexer->simple_token = NEXT;
            else if (strcmp("true", word) == 0)
                lexer->simple_token = TRUE;
            else if (strcmp("false", word) == 0)
                lexer->simple_token = FALSE;
            else if (strcmp("not", word) == 0)
                lexer->simple_token = NOT_TOK;
            else if (strcmp("and", word) == 0)
                lexer->simple_token = AND_TOK;
            else if (strcmp("or", word) == 0)
                lexer->simple_token = OR_TOK;
            // If it's not a keyword, then it's a variable
            else
            {
                lexer->type = VARIABLE_TOKEN;
                lexer->variable = word;
            }
        }
        else
        {
            sprintf(error, "Error at line %d, column %d: unknown character: %c", line_number, column_number, c);
            return error;
        }
        lexer->next = malloc(sizeof(struct Lexer));
        lexer = lexer->next;
    }

    // Treating the last extra lexer node as an "end of stream" node since we've malloc'd it anyways
    // Maybe should just free it / null it out, but I think that might cause issues.
    lexer->column_number = column_number;
    lexer->line_number = line_number;
    lexer->type = SIMPLE_TOKEN;
    lexer->simple_token = END_OF_STREAM;
    return NULL;
}
