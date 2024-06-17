#include "chaintable.h"
#include "hash.h"

#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>

/*
A simple chaining hashtable. 

TODO:
- We can potentially improve this lprobingtable by using a union for chainlinks.
  Then, we can just mark nodes deleted and potentially reuse them. 
*/

static bool chainlink_is_empty(const struct chainlink link)
{
        return (intptr_t)(link.next) == (intptr_t)CHAINLINK_EMPTY;
}

static bool chainlink_is_leaf(const struct chainlink link)
{
        return (intptr_t)(link.next) == (intptr_t)CHAINLINK_LEAF;
}

void chaintable_init(chaintable *tbl)
{
        chaintable_init_with_capacity(tbl, 0, 0.8);
}

void chaintable_init_with_capacity(chaintable *tbl, size_t cap, double ldf)
{
        tbl->buf = NULL;
        if (cap > 0) {
                tbl->buf = (struct chainlink *)calloc(cap * 2, sizeof(struct chainlink));
                if (tbl->buf == NULL) {
                        puts("OOM");
                        exit(1);
                }
        }
        tbl->cap = cap;
        tbl->ldf = ldf;
        tbl->len = 0;
}

void chaintable_insert(chaintable *tbl, keyint_t k, valint_t v)
{
        if (chaintable_load(tbl) >= tbl->ldf) {
                chaintable_realloc(tbl, tbl->cap * 2);
        }
        size_t h = idhash((char *)&k, sizeof(keyint_t));
        size_t i = h % tbl->cap;
        for (struct chainlink *link = tbl->buf + i;; link = link->next) {
                if (chainlink_is_empty(*link)) {
                        *link = (struct chainlink){
                                .next = CHAINLINK_LEAF,
                                .key = k,
                                .val = v,
                        };
                        break;
                } else if (link->key == k) {
                        link->val = v;
                        break;
                } else if (chainlink_is_leaf(*link)) {
                        link->next = tbl->buf + tbl->cap + tbl->len;
                        *link->next = (struct chainlink){
                                .next = CHAINLINK_LEAF,
                                .key = k,
                                .val = v,
                        };
                        tbl->len++;
                        break;
                }
        }
}

valint_t *chaintable_lookup(chaintable *tbl, keyint_t k)
{
        if (tbl->len == 0) {
                return NULL;
        }
        size_t h = idhash((char *)&k, sizeof(keyint_t));
        size_t i = h % tbl->cap;
        for (struct chainlink *link = tbl->buf + i;; link = link->next) {
                if (chainlink_is_empty(*link)) {
                        return NULL;
                } else if (link->key == k) {
                        return &link->val;
                } else if (chainlink_is_leaf(*link)) {
                        return NULL;
                }
        }
}

void chaintable_remove(chaintable *tbl, keyint_t k)
{
        if (tbl->len == 0) {
                return;
        }
        size_t h = idhash((char *)&k, sizeof(keyint_t));
        size_t i = h % tbl->cap;

        struct chainlink *prevlink = tbl->buf + i, *link;
        if (chainlink_is_empty(*prevlink)) {
                return;
        }
        if (chainlink_is_leaf(*prevlink)) {
                prevlink->next = CHAINLINK_EMPTY;
                return;
        }
        link = prevlink->next;
        for (; !chainlink_is_leaf(*link); prevlink = link, link = link->next) {
                if (link->key == k) {
                        if (chainlink_is_leaf(*link->next)) {
                                prevlink->next = CHAINLINK_LEAF;
                        } else {
                                prevlink->next = link->next;
                        }
                        return;
                }
        }
}

void chaintable_deinit(chaintable *tbl)
{
        if (tbl->buf) {
                free(tbl->buf);
        }
        tbl->cap = 0;
        tbl->ldf = 0;
        tbl->len = 0;
}

void chaintable_realloc(chaintable *tbl, size_t cap)
{
        chaintable newtbl;
        chaintable_init_with_capacity(&newtbl, cap == 0 ? 8 : cap * 4, tbl->ldf);
        for (size_t i = 0; i < tbl->cap; ++i) {
                struct chainlink link = tbl->buf[i];
                if (chainlink_is_empty(link)) {
                        continue;
                }
                for (;; link = *link.next) {
                        chaintable_insert(&newtbl, link.key, link.val);
                        if (chainlink_is_leaf(link)) {
                                break;
                        }
                }
        }
        chaintable_deinit(tbl);
        *tbl = newtbl;
}

double chaintable_load(const chaintable *tbl)
{
        return tbl->cap == 0 ? 1.0 : (double)tbl->len / (double)tbl->cap;
};

struct astv_array_vtable chaintable_vtable()
{
        return (struct astv_array_vtable){
                .init = (astv_array_init_fn_t)chaintable_init,
                .insert = (astv_array_insert_fn_t)chaintable_insert,
                .remove = (astv_array_remove_fn_t)chaintable_remove,
                .lookup = (astv_array_lookup_fn_t)chaintable_lookup,
                .deinit = (astv_array_deinit_fn_t)chaintable_deinit,
        };
}