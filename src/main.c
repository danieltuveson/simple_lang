#include <stdlib.h>
#include "io.h"
#include "log.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        repl();
    }
    else if (argc == 2)
    {
        run_file(argv[1]);
    }
    else
    {
        exit(EXIT_FAILURE);
    }
    
    log("finished\n");
    return 0;
}
