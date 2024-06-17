#include "lprobingtable.h"

#include <stdint.h>
#include <stdio.h>

void lprobingtable_init(lprobingtable *tbl)
{
        lprobingtable_init_with_capacity(tbl, 0, 0.8);
}

int lprobingtable_init_with_capacity(lprobingtable *tbl, const size_t capacity,
                                     const double load_factor)
{
        tbl->buf = NULL;
        if (capacity > 0) {
                tbl->buf = (struct entry *)calloc(capacity, sizeof(struct entry));
                if (tbl->buf == NULL) {
                        return ENOMEM;
                }
        }
        tbl->nr_occupied = 0;
        tbl->capacity = capacity;
        tbl->load_factor = load_factor;
        tbl->nr_probes = 0;
        return 0;
}

struct entry *lprobingtable_lookup_slot(lprobingtable *tbl, keyint_t k)
{
        size_t h = idhash((char *)&k, sizeof(keyint_t));
#ifdef USE_LINEAR_PROBING
        for (size_t i = h % tbl->capacity;; i = (i + 1) % tbl->capacity) {
                struct entry *e = &tbl->buf[i];
#else
#ifdef USE_QUADRATIC_PROBING
        size_t start = h % tbl->capacity;
        for (size_t i = 0;; ++i) {
                struct entry *e = &tbl->buf[(start + i * i) % tbl->capacity];
#else
#ifdef USE_RANDOM_PROBING
        // This doens't work because we are dotting around randomly, sowe can't guarante even distribution among unvisitied
        size_t start = h % tbl->capacity;
        for (size_t i = 0;; ++i) {
                size_t salt = h + i;
                size_t probei = idhash(&salt, sizeof(size_t)) % (tbl->capacity - i);
                struct entry *e = &tbl->buf[(start + i + probei) % tbl->capacity];
#else
#error no probing strategy selected
#endif
#endif
#endif
                tbl->nr_probes++;
                if (e->meta == CTRL_EMPTY) {
                        return e;
                } else if (e->meta == CTRL_DEL) {
                        continue;
                } else if (e->key == k) {
                        return e;
                }
        }
}

void lprobingtable_insert(lprobingtable *tbl, keyint_t k, valint_t v)
{
        if (lprobingtable_load(tbl) >= tbl->load_factor) {
                lprobingtable_realloc(tbl, tbl->capacity ? tbl->capacity * 2 : 8);
        }
        struct entry *e = lprobingtable_lookup_slot(tbl, k);
        if (e->meta == CTRL_EMPTY) {
                e->meta = CTRL_PRESENT;
                e->key = k;
                e->val = v;
                tbl->nr_occupied++;
        } else {
                e->val = v;
        }
}

valint_t *lprobingtable_lookup(lprobingtable *tbl, keyint_t k)
{
        if (tbl->capacity == 0) {
                return NULL;
        }
        struct entry *e = lprobingtable_lookup_slot(tbl, k);
        if (e->meta == CTRL_EMPTY) {
                return NULL;
        } else {
                return &e->val;
        }
}

void lprobingtable_remove(lprobingtable *tbl, keyint_t k)
{
        struct entry *e = lprobingtable_lookup_slot(tbl, k);
        if (e->meta == CTRL_PRESENT) {
                e->meta = CTRL_DEL;
                tbl->nr_occupied--;
        }
}

void lprobingtable_deinit(lprobingtable *tbl)
{
        if (tbl->buf) {
                free(tbl->buf);
        }
        tbl->buf = NULL;
        tbl->capacity = 0;
        tbl->nr_occupied = 0;
}

void lprobingtable_realloc(lprobingtable *tbl, size_t new_capacity)
{
        lprobingtable new_tbl;
        int err = lprobingtable_init_with_capacity(&new_tbl, new_capacity, tbl->load_factor);
        if (err) {
                puts("OOM");
                exit(1);
        }
        for (size_t i = 0; i < tbl->capacity; ++i) {
                struct entry e = tbl->buf[i];
                if (e.meta == CTRL_PRESENT) {
                        lprobingtable_insert(&new_tbl, e.key, e.val);
                }
        }
        lprobingtable_deinit(tbl);
        *tbl = new_tbl;
}

double lprobingtable_load(const lprobingtable *tbl)
{
        return tbl->capacity == 0 ? 1.0 : (double)tbl->nr_occupied / (double)tbl->capacity;
}

struct astv_array_vtable lprobingtable_vtable()
{
        return (struct astv_array_vtable){
                .init = (astv_array_init_fn_t)lprobingtable_init,
                .insert = (astv_array_insert_fn_t)lprobingtable_insert,
                .remove = (astv_array_remove_fn_t)lprobingtable_remove,
                .lookup = (astv_array_lookup_fn_t)lprobingtable_lookup,
                .deinit = (astv_array_deinit_fn_t)lprobingtable_deinit,
        };
}