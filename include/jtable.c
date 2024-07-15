#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "jtable.h"

#define LDF 0.8

size_t hash(keyint_t k)
{
        return idhash((char *)&k, sizeof(keyint_t));
}

void jtable_init_with_capacity(jtable *self, size_t cap)
{
        self->buckets = calloc(cap, sizeof(struct bucket));
        self->len = 0;
        self->cap = cap;
}

void jtable_init(jtable *self)
{
        self->buckets = NULL;
        self->len = 0;
        self->cap = 0;
}

void bucket_print(struct bucket b)
{
        switch (b.ctrl) {
        case CTRL_EMPTY:
                printf("e[]");
                break;
        case CTRL_DISPLACED:
                printf("d[");
                break;
        case CTRL_SNUG:
                printf("s[");
                break;
        }
        if (b.ctrl != CTRL_EMPTY) {
                printf("%lu: %lu] %d <-> %d => %d", b.key, b.val, b.prev, b.next, b.chain_start);
        }
}

void jtable_print(jtable *self)
{
        printf("jtable {\n");
        for (size_t i = 0; i < self->cap; ++i) {
                printf("  ");
                bucket_print(self->buckets[i]);
                printf(",\n");
        }
        printf("}\n");
}

size_t jtable_probe_until_empty(jtable *self, size_t i)
{
        while (self->buckets[i].ctrl != CTRL_EMPTY) {
                i = (i + 1) % self->cap;
        }
        return i;
}

void jtable_realloc(jtable *self)
{
        jtable newtbl;
        jtable_init_with_capacity(&newtbl, self->cap ? self->cap * 2 : 6);
        for (size_t i = 0; i < self->cap; ++i) {
                struct bucket b = self->buckets[i];
                if (b.ctrl != CTRL_EMPTY) {
                        jtable_insert(&newtbl, b.key, b.val);
                }
        }
        jtable_deinit(self);
        *self = newtbl;
}

inline long jtable_follow_chain(jtable *self, keyint_t k, long i)
{
        struct bucket *b = &self->buckets[i];
        i = (i + b->chain_start) % self->cap;
        b = &self->buckets[i];
        // Follow the chain until the end
        do {
                if (b->key == k) {
                        // Update in-place, if we find a match along the way
                        return i;
                }
                i = (i + b->next) % self->cap;
                b = &self->buckets[i];
        } while (b->next != 0);
        return i;
}

void jtable_insert(jtable *self, keyint_t k, valint_t v)
{
        if (self->len >= 3 * self->cap / 4) {
                jtable_realloc(self);
        }
        size_t h = hash(k);
        long i = h % self->cap;
        struct bucket *b = &self->buckets[i];
        if (b->ctrl == CTRL_EMPTY) {
                // We can immediately insert into the bucket
                b->ctrl = CTRL_SNUG;
                b->key = k;
                b->val = v;
                self->len++;
                return;
        }
        if (b->ctrl == CTRL_DISPLACED && b->chain_start == 0) {
                // No chain exists for this hash, and this cell is already
                // occupied
                long j = jtable_probe_until_empty(self, i);
                b->chain_start = (j - i) % self->cap;
                b = &self->buckets[j];
                b->ctrl = CTRL_DISPLACED;
                b->key = k;
                b->val = v;
                self->len++;
                return;
        }
        // else: A chain exists for this hash, but it starts elsewhere
        i = jtable_follow_chain(self, k, i);
        b = &self->buckets[i];
        if (b->key == k) {
                b->val = v;
                return;
        }
        // We have reached the end of a chain, without finding a match. We need
        // to probe for a new spot
        long j = jtable_probe_until_empty(self, i);
        uint16_t d = (j - i) % self->cap;
        b->next = d;
        b = &self->buckets[j];
        b->ctrl = CTRL_DISPLACED;
        b->prev = d;
        b->key = k;
        b->val = v;
        self->len++;
}

/** Lookup the index of a bucket that matches `k`. `-1` represents an absent 
  * key 
  */
inline long jtable_lookup_bucket(jtable *self, keyint_t k)
{
        size_t h = hash(k);
        long i = h % self->cap;
        struct bucket *b = &self->buckets[i];
        if (b->ctrl == CTRL_EMPTY) {
                return -1;
        }
        i = jtable_follow_chain(self, k, i);
        b = &self->buckets[i];
        if (b->key == k) {
                return i;
        }
        return -1;
}

/** For some bucket `b` that is being removed, replace it with the chain which
  * it blocks. 
  */
inline void jtable_replace_with_chain_start(jtable *self, long i)
{
        struct bucket *b = &self->buckets[i];
        if (b->chain_start) {
                long j = (i + (long)b->chain_start) % self->cap;
                struct bucket *headb = &self->buckets[j];
                headb->ctrl = CTRL_EMPTY;
                b->ctrl = CTRL_SNUG;
                if (headb->next) {
                        b->next = headb->next + b->chain_start;
                } else {
                        b->next = 0;
                }
                b->chain_start = 0;
                b->prev = 0;
                b->key = headb->key;
                b->val = headb->val;
                if (headb->chain_start) {
                        jtable_replace_with_chain_start(self, j);
                }
        }
}

void jtable_remove(jtable *self, keyint_t k)
{
        if (self->cap == 0) {
                return;
        }
        long i = jtable_lookup_bucket(self, k);
        if (i == -1) {
                return;
        }
        struct bucket *b = &self->buckets[i];

        if (b->ctrl == CTRL_SNUG) {
                if (!b->next) {
                        b->ctrl = CTRL_EMPTY;
                        return;
                }
                long j = (i + (long)b->next) % self->cap;
                struct bucket *nextb = &self->buckets[j];
                nextb->ctrl = CTRL_EMPTY;
                b->next += nextb->next;
                b->key = nextb->key;
                b->val = nextb->val;
                b->prev = 0;
                jtable_replace_with_chain_start(self, j);
                return;
        }

        b->ctrl = CTRL_EMPTY;
        if (b->prev) {
                struct bucket *prevb = &self->buckets[(i - (long)b->prev) % self->cap];
                prevb->next += b->next;
        }
        if (b->next) {
                struct bucket *nextb = &self->buckets[(i + (long)b->next) % self->cap];
                nextb->prev += b->prev;
        }
        jtable_replace_with_chain_start(self, i);
}

valint_t *jtable_lookup(jtable *self, keyint_t k)
{
        if (self->cap == 0) {
                return NULL;
        }
        long i = jtable_lookup_bucket(self, k);
        if (i == -1) {
                return NULL;
        }
        return &self->buckets[i].val;
}

void jtable_deinit(jtable *self)
{
        if (self->buckets != NULL) {
                free(self->buckets);
        }
        self->buckets = NULL;
        self->cap = 0;
        self->len = 0;
}

struct astv_array_vtable jtable_vtable()
{
        return (struct astv_array_vtable){
                .init = (astv_array_init_fn_t)jtable_init,
                .insert = (astv_array_insert_fn_t)jtable_insert,
                .remove = (astv_array_remove_fn_t)jtable_remove,
                .lookup = (astv_array_lookup_fn_t)jtable_lookup,
                .deinit = (astv_array_deinit_fn_t)jtable_deinit,
        };
}