#pragma once

#include "hash.h"
#include "astv_array_vtable.h"

#define CTRL_EMPTY ((char)0)
#define CTRL_SNUG ((char)1)
#define CTRL_DISPLACED ((char)2)

struct bucket {
        char ctrl;
        // uint16_t prev;
        uint16_t next;
        uint16_t chain_start;
        keyint_t key;
        valint_t val;
};

typedef struct {
        struct bucket *buckets;
        size_t len;
        size_t cap;
} jtable;

void jtable_init(jtable *);

void jtable_print(jtable *);

void jtable_insert(jtable *, keyint_t, valint_t);

void jtable_remove(jtable *, keyint_t);

valint_t *jtable_lookup(jtable *, keyint_t);

void jtable_deinit(jtable *);

struct astv_array_vtable jtable_vtable();