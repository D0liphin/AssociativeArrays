#pragma once

extern "C" {
#include "astv_array_vtable.h"
#include "cpp_std_unordered_map.h"
}

#include <benchmark/benchmark.h>

#include <ctime>
#include <cstdlib>
#include <stdexcept>

// 4,194,304
#define RANDOMS_LEN (1 << 22)

keyint_t randoms[RANDOMS_LEN];

void setup_test_data()
{
        srand(time(NULL));
        for (size_t i = 0; i < RANDOMS_LEN; ++i) {
                randoms[i] = rand();
        }
}

auto _ = []() {
        setup_test_data();
        return 0;
}();

template <typename T> struct BMParams {
        static struct astv_array_vtable vtbl()
        {
                throw std::runtime_error("not implemented for this class");
        }
};

template <typename T> struct BM {
        static void insert_2update_randoms(benchmark::State &state)
        {
                // get the vtable
                struct astv_array_vtable vtbl = BMParams<T>::vtbl();
                // get the table and cast to a (void *) so C++ doesn't complain
                // to us about implicit casts
                T t;
                void *tbl = (void *)&t;
                // actual benchmark code
                size_t nr_insertions = state.range(0);
                for (auto _ : state) {
                        vtbl.init(tbl);
                        for (size_t i = 0; i < nr_insertions * 3; ++i) {
                                keyint_t k = randoms[i] % nr_insertions;
                                vtbl.insert(tbl, k, 0);
                        }
                        vtbl.deinit(tbl);
                }
        }

        static void insert_remove_all_randoms(benchmark::State &state)
        {
                struct astv_array_vtable vtbl = BMParams<T>::vtbl();
                T t;
                void *tbl = (void *)&t;
                size_t nr_insertions = state.range(0);
                for (auto _ : state) {
                        vtbl.init(tbl);
                        for (size_t i = 0; i < nr_insertions; ++i) {
                                keyint_t k = randoms[i] % nr_insertions;
                                vtbl.insert(tbl, k, 0);
                        }
                        for (long i = nr_insertions; i >= 0; --i) {
                                keyint_t k = randoms[i] % nr_insertions;
                                vtbl.remove(tbl, k);
                        }
                        vtbl.deinit(tbl);
                }
        }

        static void insert_remove_lookup(benchmark::State &state)
        {
                struct astv_array_vtable vtbl = BMParams<T>::vtbl();
                T t;
                void *tbl = (void *)&t;
                size_t n = state.range(0);
                for (auto _ : state) {
                        vtbl.init(tbl);
                        size_t maxj;
                        size_t i = 0;
                        maxj = i + n;
                        for (size_t j = i; j < maxj; ++j) {
                                keyint_t k = randoms[j] % n;
                                vtbl.insert(tbl, k, 0);
                        }
                        i += n;
                        maxj = i + n / 2;
                        for (size_t j = i; j < maxj; ++j) {
                                keyint_t k = randoms[j] % n;
                                vtbl.remove(tbl, k);
                        }
                        i += n;
                        maxj = i + n / 2;
                        for (size_t j = i; j < maxj; ++j) {
                                keyint_t k = randoms[j] % n;
                                vtbl.lookup(tbl, k);
                        }
                        vtbl.deinit(tbl);
                }
        }
};

#define ASTV_ARRAY_BENCHMARK_ALL(T, VTBL)                               \
        template <> struct BMParams<T> {                                \
                static struct astv_array_vtable vtbl()                  \
                {                                                       \
                        return VTBL;                                    \
                }                                                       \
        };                                                              \
        BENCHMARK(BM<T>::insert_2update_randoms)->Range(8, 8 << 13);    \
        BENCHMARK(BM<T>::insert_remove_all_randoms)->Range(8, 8 << 13); \
        BENCHMARK(BM<T>::insert_remove_lookup)->Range(8, 8 << 13);

#define ASTV_ARRAY_RUN_BENCHMARKS() BENCHMARK_MAIN()