#include "astv_array_test.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void test_inserts_persist(struct astv_array_vtable vtbl, void *tbl)
{
        vtbl.init(tbl);
        for (keyint_t k = 0; k < 10000; ++k) {
                vtbl.insert(tbl, k, -42);
        }
        for (keyint_t k = 0; k < 10000; ++k) {
                valint_t *v = vtbl.lookup(tbl, k);
                assert(v != NULL);
                assert(*v == -42);
        }
        vtbl.deinit(tbl);
}

void test_updates_persist(struct astv_array_vtable vtbl, void *tbl)
{
        vtbl.init(tbl);
        for (keyint_t k = 0; k < 10000; ++k) {
                vtbl.insert(tbl, k, -42);
        }
        for (keyint_t k = 0; k < 10000; k += 2) {
                vtbl.insert(tbl, k, 42);
        }
        for (keyint_t k = 0; k < 10000; ++k) {
                valint_t *v = vtbl.lookup(tbl, k);
                assert(v != NULL);
                if (k % 2 == 0) {
                        assert(*v == 42);
                } else {
                        assert(*v == -42);
                }
        }
        vtbl.deinit(tbl);
}

void test_doesnt_contain_unmapped_keys(struct astv_array_vtable vtbl, void *tbl)
{
        vtbl.init(tbl);
        for (keyint_t k = 0; k < 10000; ++k) {
                vtbl.insert(tbl, k, -42);
        }
        for (keyint_t k = 10000; k < 20000; ++k) {
                assert(vtbl.lookup(tbl, k) == NULL);
        }
        vtbl.deinit(tbl);
}

void test_removes_mappings(struct astv_array_vtable vtbl, void *tbl)
{
        vtbl.init(tbl);
        for (keyint_t k = 0; k < 10000; ++k) {
                vtbl.insert(tbl, k, -42);
        }
        for (keyint_t k = 0; k < 5000; ++k) {
                vtbl.remove(tbl, k);
        }
        for (keyint_t k = 0; k < 5000; ++k) {
                assert(vtbl.lookup(tbl, k) == NULL);
        }
        for (keyint_t k = 5000; k < 10000; ++k) {
                valint_t *v = vtbl.lookup(tbl, k);
                assert(v != NULL);
                assert(*v == -42);
        }
        vtbl.deinit(tbl);
}

void test_against_oracle(struct astv_array_vtable vtbl, void *tbl)
{
        cpp_std_unordered_map oracle;
        vtbl.init(tbl);
        cpp_std_unordered_map_init(&oracle);
#define INSERT 0
#define LOOKUP 1
#define REMOVE 2
        // #define CLEAR 4
        int maxv = 1 << 19;
        for (size_t it = 0; it < (1 << 23); ++it) {
                if (it % 4093 == 0) {
                        for (int n = 0; n < 10; ++n) {
                                printf("\b\b\b\b\b");
                        }
                        printf("tested %lu operations against oracle...", it);
                        fflush(stdout);
                }
                int fn = rand() % (REMOVE + 1);
                int k, v;
                switch (fn) {
                case INSERT: {
                        k = rand() % maxv;
                        v = rand() % maxv;
                        vtbl.insert(tbl, k, v);
                        cpp_std_unordered_map_insert(&oracle, k, v);
                        break;
                }
                case LOOKUP: {
                        k = rand() % maxv;
                        valint_t *testv = vtbl.lookup(tbl, k);
                        valint_t *oraclev = cpp_std_unordered_map_lookup(&oracle, k);
                        if (testv == NULL) {
                                assert(oraclev == NULL);
                                break;
                        }
                        // printf("%ld == %ld\n", *testv, *oraclev);
                        assert(*testv == *oraclev);
                        break;
                }
                case REMOVE: {
                        k = rand() % maxv;
                        vtbl.remove(tbl, k);
                        cpp_std_unordered_map_remove(&oracle, k);
                        break;
                }
                }
        }
        puts("");
        cpp_std_unordered_map_deinit(&oracle);
        vtbl.deinit(tbl);
}

#define ANSIBLUE(STR) "\033[1m\033[34m" STR "\033[0m"
#define ANSIGREEN(STR) "\033[1m\033[32m" STR "\033[0m"

void run_all_tests(const char *name, struct astv_array_vtable vtbl, void *tbl)
{
#define RUNTEST(testfn)                                                            \
        ({                                                                         \
                printf("start " ANSIBLUE("%s") "::" ANSIBLUE(#testfn) "\n", name); \
                testfn(vtbl, tbl);                                                 \
                printf(ANSIGREEN("%s") "::" ANSIGREEN(#testfn) " ✅\n", name);      \
        })
        RUNTEST(test_inserts_persist);
        RUNTEST(test_updates_persist);
        RUNTEST(test_doesnt_contain_unmapped_keys);
        RUNTEST(test_removes_mappings);
        RUNTEST(test_against_oracle);
}