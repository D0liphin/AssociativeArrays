#include "astv_array_vtable.h"

#include <stdint.h>
#include <stddef.h>

#define CHAINLINK_LEAF ((struct chainlink *)_Alignof(struct chainlink))
#define CHAINLINK_EMPTY ((struct chainlink *)NULL)

struct chainlink {
        /* 
         * one of
         * - `CHAINLINK_LEAF`: this is the leaf node of this chain
         * - `CHAINLINK_EMPTY`: this chain is empty
         * or a pointer to the next element in this chain
         */
        struct chainlink *next;
        keyint_t key;
        valint_t val;
};

typedef struct {
        /*
         * a buffer in two parts. the first `cap` elements are for chain roots
         * the second part of the buffer acts as a space for allocating new 
         * nodes. `len` is used for both the load calculations, and for keeping
         * track of the initialized region of the second part.
         */
        struct chainlink *buf;
        /* the capacity of the buffer */
        size_t cap;
        /* the number of entries that have been inserted */
        size_t len;
        /* the load factor */
        double ldf;
} chaintable;

void chaintable_init(chaintable *);

void chaintable_init_with_capacity(chaintable *, size_t, double);

void chaintable_insert(chaintable *, keyint_t, valint_t);

valint_t *chaintable_lookup(chaintable *, keyint_t);

void chaintable_remove(chaintable *, keyint_t);

void chaintable_deinit(chaintable *);

struct astv_array_vtable chaintable_vtable();

double chaintable_load(const chaintable *);

void chaintable_realloc(chaintable *, size_t);