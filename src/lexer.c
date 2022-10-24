#include "lexer.h"
#include "error.h"
#include "log.h"

// Break string into tokens
// If there's an error, stop lexing
char *tokens(struct Lexer *lexer, char *string)
{
    int num, i, capacity;
    char c, prevc;
    char *str;
    unsigned long line_number, column_number;
    line_number = 1;
    column_number = 1;

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

        // Comment
        else if (c == '\'')
        {
            capacity = 10;
            prevc = '\0';
            str = calloc(capacity, sizeof(char));
            i = 0;
            for (c = *string; c != '\0' && c != '\n'; string++, c = *string)
            {
                if (i == (capacity - 1))
                {
                    str = grow_string(str, &capacity);
                }
                *(str+i) = c;
                i++;
                column_number++;
                prevc = c;

            }
            string--;
            column_number--;
            lexer->type = COMMENT_TOKEN;
            lexer->comment = str;
        }
        // String
        // TODO: fix this
        // actually... maybe this works? 
        // I think I was allocing wrong before, so maybe that was the issue
        else if (c == '"')
        {
            capacity = 10;
            prevc = '\0';
            str = calloc(capacity, sizeof(char));
            i = 0;
            string++;
            column_number++;
            for (c = *string; 
                    (c != '"') || (prevc == '\\' && c == '"'); 
                    string++, c = *string)
            {
                if (i == (capacity - 1))
                {
                    str = grow_string(str, &capacity);
                }

                if (c == '\n')
                {
                    return error(line_number, column_number, "newline in string literal");
                }
                else if (c =='\0')
                {
                    return error(line_number, column_number, "end of file reached before closing of string literal");
                }
                else if (prevc == '\\') 
                {
                    switch (c)
                    {
                        case '\\':
                            *(str+i) = '\\';
                            c = '\0'; // set to garbage so we don't escape a '"' or 'n'
                            break;
                        case '"':
                            *(str+i) = '"';
                            break;
                        case 'n':
                            *(str+i) = '\n';
                            break;
                        default:
                            return error(line_number, column_number, "unknown escape sequence '\\%c'", c);
                    }
                    i++;
                }
                else if (c != '\\')
                {
                    *(str+i) = c;
                    i++;
                }
                column_number++;
                prevc = c;
            }
            column_number++;
            lexer->type = STRING_TOKEN;
            lexer->string = str;
            // printf("%c\n",c);
        }

        // Int
        else if (isdigit(c))
        {
            num = 0;
            i = 0;
            for (c = *string; !is_delimiter(c); string++, c = *string)
            {
                if (!isdigit(c))
                    return error(line_number, column_number, "alphanumeric may not begin with a digit");
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
                    return error(line_number, column_number, "unknown character in token: '%c'", c);
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
            if (strcmp("dim", word) == 0)
                lexer->simple_token = DIM;
            else if (strcmp("if", word) == 0)
                lexer->simple_token = IF_TOK;
            else if (strcmp("elseif", word) == 0)
                lexer->simple_token = ELSEIF;
            else if (strcmp("then", word) == 0)
                lexer->simple_token = THEN;
            else if (strcmp("else", word) == 0)
                lexer->simple_token = ELSE_TOK;
            else if (strcmp("end", word) == 0)
                lexer->simple_token = END;
            else if (strcmp("while", word) == 0)
                lexer->simple_token = WHILE_TOK;
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
            return error(line_number, column_number, "unknown character: '%c'", c);
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

char *grow_string(char *str, int *capacity)
{
    char *old_str;
    *capacity *= 2;
    old_str = str;
    str = calloc(*capacity, sizeof(char));
    strcpy(str, old_str);
    free(old_str);
    return str;
}
