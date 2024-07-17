#include "../include/jtable.h"
#include "../include/astv_array_test.h"
#include <stdio.h>
#include <assert.h>

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 6, 0);
//         vtbl.insert(&tbl, 0, 1);
//         valint_t *v = vtbl.lookup(&tbl, 0);
//         assert(v != NULL);
//         assert(*v == 1);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tblt;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 6, 0);
//         vtbl.insert(&tbl, 0, 0);
//         vtbl.remove(&tbl, 6);
//         vtbl.remove(&tbl, 0);
//         assert(vtbl.lookup(&tbl, 0) == NULL);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 6, 0);
//         vtbl.insert(&tbl, 0, 0);
//         vtbl.insert(&tbl, 1, 0);
//         vtbl.remove(&tbl, 1);
//         assert(vtbl.lookup(&tbl, 1) == NULL);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 6, 0);
//         vtbl.insert(&tbl, 0, 0);
//         vtbl.insert(&tbl, 7, 0);
//         vtbl.insert(&tbl, 1, 0);
//         vtbl.remove(&tbl, 7);
//         valint_t *v = vtbl.lookup(&tbl, 1);
//         assert(v != NULL);
//         assert(*v == 0);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 6, 0);
//         vtbl.insert(&tbl, 0, 0);
//         vtbl.insert(&tbl, 1, 0);
//         vtbl.insert(&tbl, 7, 0);
//         vtbl.remove(&tbl, 7);
//         valint_t *v = vtbl.lookup(&tbl, 1);
//         assert(v != NULL);
//         assert(*v == 0);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 6, 0);
//         vtbl.insert(&tbl, 0, 0);
//         vtbl.insert(&tbl, 1, 0);
//         vtbl.insert(&tbl, 7, 0);
//         vtbl.remove(&tbl, 1);
//         assert(vtbl.lookup(&tbl, 1) == NULL);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 6, 0);
//         vtbl.insert(&tbl, 0, 0);
//         vtbl.insert(&tbl, 7, 0);
//         vtbl.insert(&tbl, 1, 0);
//         vtbl.remove(&tbl, 6);
//         vtbl.remove(&tbl, 1);
//         assert(vtbl.lookup(&tbl, 1) == NULL);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 2, 0);
//         vtbl.insert(&tbl, 3, 0);
//         vtbl.insert(&tbl, 5, 0);
//         vtbl.insert(&tbl, 5, 1);
//         vtbl.insert(&tbl, 11, 0);
//         vtbl.remove(&tbl, 5);
//         vtbl.insert(&tbl, 12, 1);
//         vtbl.insert(&tbl, 7, 1);
//         assert(vtbl.lookup(&tbl, 0) == NULL);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 13, 0);
//         vtbl.insert(&tbl, 12, 0);
//         vtbl.insert(&tbl, 6, 0);
//         vtbl.remove(&tbl, 11);
//         vtbl.remove(&tbl, 14);
//         vtbl.insert(&tbl, 0, 1);
//         vtbl.remove(&tbl, 12);
//         vtbl.remove(&tbl, 0);
//         assert(vtbl.lookup(&tbl, 0) == NULL);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.remove(&tbl, 13);
//         vtbl.insert(&tbl, 0, 1);
//         vtbl.remove(&tbl, 15);
//         vtbl.insert(&tbl, 12, 1);
//         vtbl.insert(&tbl, 13, 0);
//         vtbl.insert(&tbl, 2, 1);
//         vtbl.insert(&tbl, 1, 1);
//         vtbl.remove(&tbl, 13);
//         valint_t *v = vtbl.lookup(&tbl, 1);
//         assert(v != NULL);
//         assert(*v == 1);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 12, 1);
//         vtbl.insert(&tbl, 6, 1);
//         vtbl.insert(&tbl, 0, 1);
//         vtbl.remove(&tbl, 12);
//         vtbl.remove(&tbl, 13);
//         vtbl.remove(&tbl, 12);
//         vtbl.remove(&tbl, 3);
//         vtbl.remove(&tbl, 0);
//         vtbl.insert(&tbl, 12, 0);
//         assert(vtbl.lookup(&tbl, 0) == NULL);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 12, 0);
//         vtbl.insert(&tbl, 6, 1);
//         vtbl.insert(&tbl, 0, 0);
//         vtbl.remove(&tbl, 6);
//         vtbl.remove(&tbl, 0);
//         vtbl.insert(&tbl, 1, 1);
//         vtbl.remove(&tbl, 1);
//         vtbl.insert(&tbl, 10, 0);
//         vtbl.remove(&tbl, 9);
//         assert(vtbl.lookup(&tbl, 0) == NULL);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.remove(&tbl, 12);
//         vtbl.remove(&tbl, 10);
//         vtbl.remove(&tbl, 5);
//         vtbl.insert(&tbl, 1, 1);
//         vtbl.insert(&tbl, 12, 1);
//         vtbl.remove(&tbl, 7);
//         vtbl.insert(&tbl, 0, 0);
//         vtbl.insert(&tbl, 13, 0);
//         vtbl.insert(&tbl, 4, 1);
//         vtbl.insert(&tbl, 12, 1);
//         vtbl.remove(&tbl, 1);
//         vtbl.insert(&tbl, 2, 0);
//         vtbl.insert(&tbl, 0, 1);
//         vtbl.insert(&tbl, 15, 1);
//         vtbl.insert(&tbl, 1, 1);
//         vtbl.remove(&tbl, 2);
//         valint_t *v = vtbl.lookup(&tbl, 1);
//         assert(v != NULL);
//         assert(*v == 1);
//         vtbl.deinit(&tbl);
//         return 0;
// }

// // Failed to match oracle output on below test case
// int main()
// {
//         jtable tbl;
//         struct astv_array_vtable vtbl = jtable_vtable();
//         vtbl.init(&tbl);
//         vtbl.insert(&tbl, 10, 0);
//         vtbl.insert(&tbl, 31, 1);
//         vtbl.remove(&tbl, 31);
//         vtbl.insert(&tbl, 22, 0);
//         vtbl.remove(&tbl, 16);
//         vtbl.insert(&tbl, 27, 1);
//         vtbl.insert(&tbl, 23, 0);
//         vtbl.remove(&tbl, 19);
//         vtbl.insert(&tbl, 11, 0);
//         vtbl.remove(&tbl, 15);
//         vtbl.remove(&tbl, 9);
//         vtbl.insert(&tbl, 24, 0);
//         vtbl.insert(&tbl, 0, 1);
//         vtbl.remove(&tbl, 31);
//         vtbl.insert(&tbl, 22, 0);
//         vtbl.remove(&tbl, 26);
//         vtbl.remove(&tbl, 6);
//         vtbl.remove(&tbl, 13);
//         vtbl.insert(&tbl, 11, 0);
//         vtbl.insert(&tbl, 10, 0);
//         vtbl.remove(&tbl, 23);
//         vtbl.insert(&tbl, 12, 1);
//         vtbl.remove(&tbl, 11);
//         vtbl.insert(&tbl, 27, 0);
//         vtbl.insert(&tbl, 23, 0);
//         vtbl.remove(&tbl, 31);
//         valint_t *v = vtbl.lookup(&tbl, 0);
//         assert(v != NULL);
//         assert(*v == 1);
//         vtbl.deinit(&tbl);
//         return 0;
// }