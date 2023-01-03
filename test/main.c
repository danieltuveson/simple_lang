#include "stdincludes.h"
#include "tests.h"
#include "test_vect.h"
#include "vect.h"

#define ASSERT(STR, COND) \
    do {\
        printf("...testing %s...\n", (STR)); \
        if (!(COND))\
        {\
            printf("FAILURE testing %s\n");\
        }\
    } while(0)




int main()
{
    bool b;
    printf("starting tests:\n");

    struct Vect *vect;
    vect = new_vect();
    for (size_t i = 0; i < vect->capacity; i++)
    {
        assert(vect->array[i] == NULL && "hello!");
        printf("elt: %s\n", vect->array[i]);
        b = b && (vect->array[i] == NULL);
        b = false;
        printf("b: %i\n", b);
    }
    ASSERT("initialization", b);




    printf("\nfinished!\n");
    // printf("successes: %u\n", tester.successes);
    // printf("failures: %u\n",  tester.failures);
    // printf("total: %u\n",     tester.total);

    return 0;
}
