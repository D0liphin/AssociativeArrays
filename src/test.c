#include "../include/astv_array_test.h"

#include "../include/cpp_std_unordered_map.h"
#include "../include/jtable.h"

int main()
{
        RUN_ALL_TESTS(jtable, jtable_vtable());
        RUN_ALL_TESTS(cpp_std_unordered_map, cpp_std_unordered_map_vtable());
        return 0;
}