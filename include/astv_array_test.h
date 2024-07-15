#pragma once

#include "../include/astv_array_vtable.h"
#include "../include/cpp_std_unordered_map.h"

/*
 * Run all tests, printing as we go.
 */
void run_all_tests(const char *, const char *, struct astv_array_vtable, void *);

/*
 * Run all tests. The first parameter is the type of the hashtable, the second 
 * is the vtable, which should be a `struct astv_array_vtable`.
 */
#define RUN_ALL_TESTS(T, VTABLE)                          \
        ({                                                \
                T tbl;                                    \
                run_all_tests(#T, #VTABLE, VTABLE, &tbl); \
        })

void tmain();