#include "error.h"

// I guess I need to start with a comment for my regex to work...
char *error(unsigned long line_number, unsigned long column_number, const char *fmt, ...)
{
    char *error; 
    const char *str;
    unsigned long message_length, template_length;
    template_length = 100;
    for (message_length = 0, str = fmt; 
            *str != '\0'; 
            str++, message_length++);
    message_length += template_length; 

    char *message;
    // We assume that stuff being passed into fmt will be < 100 characters
    message = calloc(message_length, sizeof(char));

    va_list args;
    va_start(args, fmt);
    vsprintf(message, fmt, args);
    va_end(args);

    // Add another 100 characters to account for the line numbers + standard wording
    error = calloc(message_length + template_length, sizeof(char));
    sprintf(error, "Error at line %lu, column %lu: %s", line_number, column_number, message);
    free(message);

    return error;
}

char *parse_error(struct Lexer *lexer, const char *fmt, ...)
{
    char *err;
    va_list args;
    va_start(args, fmt);
    err = error(lexer->line_number, lexer->column_number, fmt, args);
    va_end(args);
    return err;
}
