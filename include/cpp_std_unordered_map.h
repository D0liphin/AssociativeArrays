#pragma once

#include "astv_array_vtable.h"

struct cpp_std_unordered_map {
        intptr_t _[7];
};

void cpp_std_unordered_map_init(void *tbl);

void cpp_std_unordered_map_insert(void *tbl, keyint_t k, valint_t v);

void cpp_std_unordered_map_remove(void *tbl, keyint_t k);

valint_t *cpp_std_unordered_map_lookup(void *tbl, keyint_t k);

void cpp_std_unordered_map_deinit(void *tbl);

struct astv_array_vtable cpp_std_unordered_map_vtable();