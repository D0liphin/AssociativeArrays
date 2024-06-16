#include "hash.h"

size_t idhash(char *str, size_t len)
{
        size_t h = 0;
        for (size_t i = 0; i < len; ++i) {
                char *hstr = (char *)&h;
                hstr[i % sizeof(size_t)] = str[i];
        }
        return h;
}