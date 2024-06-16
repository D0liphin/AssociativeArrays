extern "C" {
#include "astv_array_vtable.h"
#include "cpp_std_unordered_map.h"
}

#include <cstddef>
#include <unordered_map>

static_assert(sizeof(std::unordered_map<keyint_t, valint_t>) ==
              sizeof(struct cpp_std_unordered_map));

static_assert(alignof(std::unordered_map<keyint_t, valint_t>) ==
              alignof(struct cpp_std_unordered_map));

extern "C" {
void cpp_std_unordered_map_init(void *tbl)
{
        new ((std::unordered_map<keyint_t, valint_t> *)tbl)
                std::unordered_map<keyint_t, valint_t>();
}

void cpp_std_unordered_map_insert(void *tbl, keyint_t k, valint_t v)
{
        (*(std::unordered_map<keyint_t, valint_t> *)tbl)[k] = v;
}

void cpp_std_unordered_map_remove(void *tbl, keyint_t k)
{
        (*(std::unordered_map<keyint_t, valint_t> *)tbl).erase(k);
}

valint_t *cpp_std_unordered_map_lookup(void *voidtbl, keyint_t k)
{
        std::unordered_map<keyint_t, valint_t> &tbl =
                (*(std::unordered_map<keyint_t, valint_t> *)voidtbl);
        auto it = tbl.find(k);
        return it == tbl.end() ? (valint_t *)NULL : &(it->second);
}

void cpp_std_unordered_map_deinit(void *tbl)
{
        ((std::unordered_map<keyint_t, valint_t> *)tbl)->~unordered_map();
}

struct astv_array_vtable cpp_std_unordered_map_vtable()
{
        return (struct astv_array_vtable){
                .init = cpp_std_unordered_map_init,
                .insert = cpp_std_unordered_map_insert,
                .remove = cpp_std_unordered_map_remove,
                .lookup = cpp_std_unordered_map_lookup,
                .deinit = cpp_std_unordered_map_deinit,
        };
}
}
