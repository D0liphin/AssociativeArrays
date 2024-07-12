#include "../include/jtable.h"

int main()
{
        jtable tbl;
        jtable_init(&tbl);
        jtable_insert(&tbl, 5, 10);
        jtable_insert(&tbl, 5 + 8, 11);
        jtable_insert(&tbl, 6, 12);
        jtable_insert(&tbl, 6 + 8, 12);
        jtable_print(&tbl);
        return 0;
}