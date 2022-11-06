#include "vect.h"

// This is arbitrary, might change in the future
const size_t INIT_CAPACITY = 8;


struct Vect *new_vect(void)
{
    struct Vect *vect;
    vect = malloc(sizeof(*vect));
    vect->size = 0;
    vect->capacity = INIT_CAPACITY; 
    vect->array = calloc(vect->capacity, sizeof(void *)); 
    return vect;
}

void add_item(struct Vect **vect_ptr, void *item)
{
    struct Vect *vect;
    vect = *vect_ptr;
    if (vect->size == vect->capacity)
    {
        grow_vect(vect_ptr);
    }
    vect->array[(*vect_ptr)->size] = item;
    vect->size++;
}

// probably never need to shrink
// once we have the expressions we'll either evaluate or free them
void grow_vect(struct Vect **vect_ptr)
{
    struct Vect *vect, *old_vect;

    vect = new_vect();
    old_vect = *vect_ptr;

    vect->capacity = 2 * old_vect->capacity;
    vect->size = old_vect->size;
    vect->array = calloc(vect->capacity, sizeof(void *));

    for (size_t i = 0; i < vect->size; i++)
    {
        vect->array[i] = old_vect->array[i];
    }

    free(old_vect->array);
    free(old_vect);
    *vect_ptr = vect;
}
