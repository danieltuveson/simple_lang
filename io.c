#include "io.h"
#include "datatype.h"
#include "lexer.h"
#include "parser.h"
#include "error.h"
#include "log.h"

char *read_file(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error: could not open file specified.");
        exit(EXIT_FAILURE);
    }
    else
    {
        char c, *content, *content_ptr;
        fseek(fp, 0L, SEEK_END);
        int length = ftell(fp) + 1;
        rewind(fp);
        content = malloc(sizeof(char) * length);
        content_ptr = content;
        while ((c = fgetc(fp)) != EOF)
        {
            *content = c;
            content++;
        }
        (*content)++;
        *content = '\0';
        return content_ptr;
    }
}

void run_file(char *filename)
{
    char *content;
    struct Lexer *lexer;
    struct ParseStmts *parsed_stmts;

    log("...reading file contents...\n");
    content = read_file(filename);
    if (content == NULL)
    {
        fflush(stdin);
        free(content);
        return;
    }

    log("...lexing contents...\n");
    lexer = malloc(sizeof(struct Lexer));
    char *err = tokens(lexer, content);
    if (err != NULL)
    {
        printf("%s\n", err);
        return;
    }
    print_lexer(lexer);

    log("...parsing contents...\n");
    parsed_stmts = parse_stmts(lexer);
    if (parsed_stmts->type == PARSE_ERROR)
    {
        printf("error %s\n", parsed_stmts->error);
        print_statements(parsed_stmts->stmts);
        return;
    }
    print_statements(parsed_stmts->stmts);
}

void repl(void)
{
    printf("\nStarting simple_lang interpreter \npress Ctrl+D to exit\n\n");
    struct Lexer *lexer;
    struct Parser *parser;

    while (true)
    {
        printf("> ");

        char *line = read_repl();
        if (line == NULL)
        {
            fflush(stdin);
            free(line);
            continue;
        }

        // Lex input
        printf("...lexing input...\n");
        lexer = malloc(sizeof(struct Lexer));
        char *err = tokens(lexer, line);
        if (err != NULL)
        {
            printf("%s\n", err);
            continue;
        }
        // print_lexer(lexer);

        // Parse input
        // try to parse expression
        printf("...parsing input...\n");
        parser = parse_expr(lexer);
        // Expressions in the terminal must be terminated by a newline or EOF
        if (parser->type != PARSE_ERROR && (parser->lexer->type != SIMPLE_TOKEN || parser->lexer->simple_token != END_OF_STREAM))
        {
            parser->type = PARSE_ERROR;
            parser->error = error(parser->lexer->line_number, parser->lexer->column_number, "unexpected token after expression: %s", st_to_str(parser->lexer->simple_token));
        }
        if (parser->type == PARSE_ERROR)
        {
            // If it fails to parse expression, try parsing it as statements
            // printf("%s\n", parser->error);
            // continue;
            struct ParseStmts *parsed_stmts;
            parsed_stmts = parse_stmts(lexer);
            if (parsed_stmts->type == PARSE_ERROR)
            {
                printf("Error parsing statement or expression:\n%s\n\n%s\n\n", parser->error, parsed_stmts->error);
                continue;
            }
            print_statements(parsed_stmts->stmts);
        }
        else
        {
            print_expr(parser->expr);
            printf("\n");
        }

        // Evaluate input
        // TODO


        // lexer = lexReplInput();
        // if (lexer->error != NO_ERROR)
        //{
        //     printf("### Error ###\n");
        //     printError(lexer->error);
        //     printf("======================\n");
        //     fflush(stdin);
        //     deleteTokens(lexer->tokens);
        //     deleteLexer(lexer);
        //     continue;
        // }
        //  parseExpression(lexer->tokens);
        fflush(stdin);
    }
}

char *read_repl(void)
{
    unsigned int buff_size = 200;
    char c, prevc, *buff;
    buff = calloc(buff_size, sizeof(char));
    c = fgetc(stdin);
    prevc = '\n';

    // done parsing if expression followed by newline or statement (or garbage) followed by two newlines
    for (unsigned int i = 0; 
            !(c == '\n' && (prevc == '\n' || is_valid_expr(buff))); 
            i++, prevc = c, c = fgetc(stdin))
    {
        if (c == EOF)
        {
            printf("\nc ya!\n");
            exit(0);
        }
        if (i == buff_size)
        {
            printf("Exceeded maximum size of %d characters for command line input\n", buff_size);
            return NULL;
        }
        else
        {
            buff[i] = c;
        }
    }
    printf("\nfinal char'%c'\n",c);
    return buff;
}

// a function that parses a whole string to see if it is a valid expression
// I don't think this works, but I don't care if the repl is broken for now
bool is_valid_expr(char *buff)
{
    printf("...checking...\n");
    struct Lexer *lexer;
    lexer = malloc(sizeof(struct Lexer));

    char *err = tokens(lexer, buff);
    if (err != NULL)
    {
        return true;
    }

    // try to parse expression
    struct Parser *parser;
    parser = parse_expr(lexer);
    if (parser->type == PARSE_ERROR)
    {
        return false;
    }
    return true;
}
