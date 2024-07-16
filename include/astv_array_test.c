#include "astv_array_test.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

char const *struct_name = "";
char const *vtable_name = "";

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

#define INSERT 0
#define REMOVE 1
#define LOOKUP 2

void test_against_oracle_randoms_huge(struct astv_array_vtable vtbl, void *tbl)
{
        srand(time(NULL));
        cpp_std_unordered_map oracle;
        vtbl.init(tbl);
        cpp_std_unordered_map_init(&oracle);
        int maxv = 1 << 19;
        for (size_t it = 0; it < (1 << 23); ++it) {
                if (it % 4093 == 0) {
                        for (int n = 0; n < 10; ++n) {
                                printf("\b\b\b\b\b");
                        }
                        printf("tested %lu operations against oracle...", it);
                        fflush(stdout);
                }
                int fn = rand() % (3);
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
                        if (oraclev == NULL) {
                                assert(testv == NULL);
                                break;
                        }
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

struct op_insert {
        keyint_t k;
        keyint_t v;
};

struct op_remove {
        keyint_t k;
};

struct operation {
        int fn;
        union {
                struct op_insert insert;
                struct op_remove remove;
        } op;
};

#define MAX_KEYINT 8

bool operation_inc(struct operation *o)
{
        valint_t max_valint = 1;
        switch (o->fn) {
        case INSERT:
                if (o->op.insert.v == max_valint) {
                        if (o->op.insert.k == MAX_KEYINT) {
                                o->fn = REMOVE;
                                o->op.remove.k = 0;
                                return true;
                        }
                        o->op.insert.k++;
                        o->op.insert.v = 0;
                        return true;
                }
                o->op.insert.v++;
                return true;
        case REMOVE:
                if (o->op.remove.k < MAX_KEYINT) {
                        o->op.remove.k++;
                        return true;
                }
                o->fn = INSERT;
                o->op.insert.k = 0;
                o->op.insert.v = 0;
                return false;
        }
        return false; // unreachable
}

bool operations_inc(struct operation *os, size_t maxl, size_t *restrict l)
{
        for (size_t i = 0; i < maxl; ++i) {
                if (operation_inc(&os[i])) {
                        *l = *l > i ? *l : i + 1;
                        return true;
                }
        }
        printf("maxl = %lu\n", maxl);
        return false;
}

void operation_print(struct operation o)
{
        switch (o.fn) {
        case INSERT:
                printf("vtbl.insert(&tbl, %lu, %lu);", o.op.insert.k, o.op.insert.v);
                break;
        case REMOVE:
                printf("vtbl.remove(&tbl, %lu);", o.op.remove.k);
                break;
        }
}

void operation_print_small(struct operation o)
{
        switch (o.fn) {
        case INSERT:
                printf("i(%lu,%lu)", o.op.insert.k, o.op.insert.v);
                break;
        case REMOVE:
                printf("r(%lu)  ", o.op.remove.k);
                break;
        }
}

void operations_announce_failure(struct operation *os, size_t faili, keyint_t k,
                                 valint_t *expectedv)
{
        for (int n = 0; n < 20; ++n) {
                printf("\b\b\b\b\b\b\b\b\b\b");
        }
        printf("// Failed to match oracle output on below test case\n");
        char t[] = "        ";
        printf("int main()\n{\n");
        printf("%s%s tbl;", t, struct_name);
        printf("\n%sstruct astv_array_vtable vtbl = %s;", t, vtable_name);
        printf("\n%svtbl.init(&tbl);", t);
        for (size_t i = 0; i <= faili; ++i) {
                printf("\n%s", t);
                operation_print(os[i]);
        }
        if (expectedv == NULL) {
                printf("\n%sassert(vtbl.lookup(&tbl, %lu) == NULL);", t, k);
        } else {
                printf("\n%svalint_t *v = vtbl.lookup(&tbl, %lu);", t, k);
                printf("\n%sassert(v != NULL);", t);
                printf("\n%sassert(*v == %lu);", t, *expectedv);
        }
        printf("\n%svtbl.deinit(&tbl);\n%sreturn 0;", t, t);
        printf("\n}\n");
}

void operations_exec(struct astv_array_vtable vtbl, void *tbl, struct operation *os, size_t osl,
                     keyint_t max_keyint)
{
        cpp_std_unordered_map oracle;
        vtbl.init(tbl);
        cpp_std_unordered_map_init(&oracle);
        for (size_t i = 0; i < osl; ++i) {
                switch (os[i].fn) {
                case INSERT: {
                        keyint_t k = os[i].op.insert.k;
                        valint_t v = os[i].op.insert.v;
                        vtbl.insert(tbl, k, v);
                        cpp_std_unordered_map_insert(&oracle, k, v);
                        break;
                }
                case REMOVE: {
                        keyint_t k = os[i].op.remove.k;
                        vtbl.remove(tbl, k);
                        cpp_std_unordered_map_remove(&oracle, k);
                        break;
                }
                }
        }
        for (keyint_t k = 0; k < max_keyint; ++k) {
                valint_t *testv = vtbl.lookup(tbl, k);
                valint_t *oraclev = cpp_std_unordered_map_lookup(&oracle, k);
                if (testv == NULL) {
                        if (oraclev != NULL) {
                                operations_announce_failure(os, osl - 1, k, oraclev);
                                exit(0);
                        }
                        break;
                }
                if (oraclev == NULL) {
                        if (testv != NULL) {
                                operations_announce_failure(os, osl - 1, k, oraclev);
                                exit(0);
                        }
                        break;
                }
                if (*testv != *oraclev) {
                        operations_announce_failure(os, osl - 1, k, oraclev);
                        exit(0);
                }
        }
        vtbl.deinit(tbl);
        cpp_std_unordered_map_deinit(&oracle);
}

void test_against_oracle_randoms(struct astv_array_vtable vtbl, void *tbl)
{
        srand(time(NULL));
        for (size_t osl = 8; osl < 13; ++osl) {
                struct operation *os = calloc(sizeof(struct operation), osl);
                keyint_t max_keyint = 16;
                printf("testing length-%lu sequences of operations...\n", osl);
                for (size_t it = 0; it < (1 << 23); ++it) {
                        if (it % 4093 == 0) {
                                for (int n = 0; n < 10; ++n) {
                                        printf("\b\b\b\b\b");
                                }
                                printf("tested %lu sequences against oracle...", it);
                                fflush(stdout);
                        }
                        for (size_t i = 0; i < osl; ++i) {
                                struct operation o;
                                int r = rand();
                                if ((r & 1) == 1) {
                                        o.fn = REMOVE;
                                        r >>= 1;
                                        o.op.remove.k = r % max_keyint;
                                } else {
                                        o.fn = INSERT;
                                        r >>= 1;
                                        o.op.insert.v = r & 1;
                                        r >>= 1;
                                        o.op.insert.k = r % max_keyint;
                                }
                                os[i] = o;
                        }
                        operations_exec(vtbl, tbl, os, osl, max_keyint);
                }
                puts("");
                free(os);
        }
}

void test_against_oracle_ordered(struct astv_array_vtable vtbl, void *tbl)
{
#define MAX_OSL 6 // Change this is if you want a more comprehensive test suite
        struct operation os[MAX_OSL] = { 0 };
        size_t osl = 1;
        size_t i = 0;
        do {
                if (i % 100003 == 0) {
                        for (int n = 0; n < 20; ++n) {
                                printf("\b\b\b\b\b\b\b\b\b\b");
                        }
                        for (size_t i = 0; i < osl; ++i) {
                                operation_print_small(os[i]);
                        }
                        fflush(stdout);
                }
                operations_exec(vtbl, tbl, os, osl, MAX_KEYINT);
                ++i;
        } while (operations_inc(os, MAX_OSL, &osl));
        puts("");
}

#define ANSIBLUE(STR) "\033[1m\033[34m" STR "\033[0m"
#define ANSIGREEN(STR) "\033[1m\033[32m" STR "\033[0m"

void run_all_tests(const char *name, const char *vname, struct astv_array_vtable vtbl, void *tbl)
{
        struct_name = name;
        vtable_name = vname;
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
        // RUNTEST(test_against_oracle_ordered);
        RUNTEST(test_against_oracle_randoms);
        RUNTEST(test_against_oracle_randoms_huge);
}