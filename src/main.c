#include "../include/jtable.h"

int main()
{
        jtable tbl;
        jtable_init(&tbl);
        jtable_insert(&tbl, 2    , 42);
        jtable_insert(&tbl, 2 + 8, 42);
        jtable_insert(&tbl, 3    , 42);
        jtable_insert(&tbl, 4    , 42);
        jtable_print(&tbl);
        jtable_remove(&tbl, 2);
        jtable_print(&tbl);
        return 0;
}