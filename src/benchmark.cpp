#include "../include/astv_array_benchmark.hpp"

extern "C" {
#include "../include/cpp_std_unordered_map.h"
#include "../include/jtable.h"
}

ASTV_ARRAY_BENCHMARK_ALL(cpp_std_unordered_map, cpp_std_unordered_map_vtable());
ASTV_ARRAY_BENCHMARK_ALL(jtable, jtable_vtable());

ASTV_ARRAY_RUN_BENCHMARKS();