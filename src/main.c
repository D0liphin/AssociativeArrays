#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../include/test.h"
#include "../include/lprobingtable.h"

int main()
{
        srand(time(NULL));
        RUN_ALL_TESTS(cpp_std_unordered_map, cpp_std_unordered_map_vtable());
        return 0;
}
