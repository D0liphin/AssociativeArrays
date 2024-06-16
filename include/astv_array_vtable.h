#pragma once

#include <stdint.h>

typedef intptr_t keyint_t;
typedef intptr_t valint_t;

typedef void (*astv_array_init_fn_t)(void *);
typedef void (*astv_array_insert_fn_t)(void *, keyint_t, valint_t);
typedef void (*astv_array_remove_fn_t)(void *, keyint_t);
typedef valint_t *(*astv_array_lookup_fn_t)(void *, keyint_t);
typedef void (*astv_array_deinit_fn_t)(void *);

/*
 * An associative array interface. Definitions are mostly taken from Wikipedia.
 * https://en.wikipedia.org/wiki/Associative_array
 */
struct astv_array_vtable {
        /* 
         * Initialize the collection.  
         * The argument to this operation is a pointer to the collection.
         */
        astv_array_init_fn_t init;
        /* 
         * Add a new (key, value) pair to the collection, mapping the key to its
         * new value. Any existing mapping is overwritten.   
         * The arguments to this operation are a pointer to the collection, the 
         * key and the value.
         */
        astv_array_insert_fn_t insert;
        /*
         * Remove a (key, value) pair from the collection, unmapping a given key
         * from its value.  
         * The argument to this operation is the key.
         */
        astv_array_remove_fn_t remove;
        /*
         * Find the value (if any) that is bound to a given key.  
         * The argument to this operation is the key. A pointer to the value is
         * returned from the operation.  
         * If no such value exists in the collection, `NULL` is returned.
         */
        astv_array_lookup_fn_t lookup;
        /*
         * Deinitialize the collection.
         * The argument to this operation is a pointer to the collection. 
         */
        astv_array_deinit_fn_t deinit;
};