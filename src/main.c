#include "../include/jtable.h"
#include "../include/astv_array_test.h"
#include <stdio.h>
#include <assert.h>

// Failed to match oracle output on below test case
int main()
{
        jtable tbl;
        struct astv_array_vtable vtbl = jtable_vtable();
        vtbl.init(&tbl);
        vtbl.insert(&tbl, 6, 0);
        vtbl.insert(&tbl, 0, 1);
        valint_t *v = vtbl.lookup(&tbl, 0);
        assert(v != NULL);
        assert(*v == 1);
        vtbl.deinit(&tbl);
        return 0;
}