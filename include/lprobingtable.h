#pragma once

#include "hash.h"
#include "astv_array_vtable.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>

#define USE_LINEAR_PROBING
// #define USE_QUADRATIC_PROBING
// #define USE_RANDOM_PROBING

typedef char ctrl_t;

#define CTRL_EMPTY 0
#define CTRL_DEL 1
#define CTRL_PRESENT -1

typedef intptr_t keyint_t;
typedef intptr_t valint_t;

struct entry {
        ctrl_t meta;
        keyint_t key;
        valint_t val;
};

typedef struct {
        struct entry *buf;
        // How many entries are currently occupied?
        size_t nr_occupied;
        size_t capacity;
        double load_factor;
        size_t nr_probes;
} lprobingtable;

void lprobingtable_init(lprobingtable *);

int lprobingtable_init_with_capacity(lprobingtable *, size_t, double);

struct entry *lprobingtable_lookup_slot(lprobingtable *, keyint_t);

void lprobingtable_insert(lprobingtable *, keyint_t, valint_t);

valint_t *lprobingtable_lookup(lprobingtable *, keyint_t);

void lprobingtable_remove(lprobingtable *, keyint_t);

void lprobingtable_deinit(lprobingtable *);

void lprobingtable_realloc(lprobingtable *, size_t);

double lprobingtable_load(const lprobingtable *);

struct astv_array_vtable lprobingtable_vtable();