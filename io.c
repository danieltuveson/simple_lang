#include "io.h"
#include "lexer.h"

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
    struct Lexer *lexer;
    char *content;
    content = read_file(filename);
    if (content == NULL)
    {
        fflush(stdin);
        free(content);
        return;
    }

    lexer = malloc(sizeof(struct Lexer));
    char *err = tokens(lexer, content);
    if (err == NULL)
    {
        print_lexer(lexer);
    }
    else
    {
        printf("%s\n", err);
    }
}

void repl(void)
{
    printf("\nStarting simple_lang interpreter \npress Ctrl+D to exit\n\n");
    struct Lexer *lexer;

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

        lexer = malloc(sizeof(struct Lexer));
        char *err = tokens(lexer, line);
        if (err == NULL)
        {
            print_lexer(lexer);
        }
        else
        {
            printf("%s\n", err);
        }

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
    char c, *buff;
    buff = calloc(buff_size, sizeof(char));
    c = fgetc(stdin);

    for (unsigned int i = 0; c != '\n'; i++, c = fgetc(stdin))
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
    return buff;
}
