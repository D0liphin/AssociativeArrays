#pragma once

#include <stdint.h>

typedef intptr_t keyint_t;
typedef intptr_t valint_t;

typedef void (*astv_array_init_fn)(void *);
typedef void (*astv_array_insert_fn)(void *, keyint_t, valint_t);
typedef void (*astv_array_remove_fn)(void *, keyint_t);
typedef valint_t *(*astv_array_lookup_fn)(void *, keyint_t);
typedef void (*astv_array_deinit_fn)(void *);

/*
 * An associative array interface. Definitions are mostly taken from Wikipedia.
 * https://en.wikipedia.org/wiki/Associative_array
 */
struct astv_array_vtable {
        /* 
         * Initialize the collection.  
         * The argument to this operation is a pointer to the collection.
         */
        void (*init)(void *);
        /* 
         * Add a new (key, value) pair to the collection, mapping the key to its
         * new value. Any existing mapping is overwritten.   
         * The arguments to this operation are a pointer to the collection, the 
         * key and the value.
         */
        void (*insert)(void *, keyint_t, valint_t);
        /*
         * Remove a (key, value) pair from the collection, unmapping a given key
         * from its value.  
         * The argument to this operation is the key.
         */
        void (*remove)(void *, keyint_t);
        /*
         * Find the value (if any) that is bound to a given key.  
         * The argument to this operation is the key. A pointer to the value is
         * returned from the operation.  
         * If no such value exists in the collection, `NULL` is returned.
         */
        valint_t *(*lookup)(void *, keyint_t);
        /*
         * Deinitialize the collection.
         * The argument to this operation is a pointer to the collection. 
         */
        void (*deinit)(void *);
};