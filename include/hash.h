#pragma once

#include <stddef.h>

/*
 * Pack `str` into a `size_t` by `XOR` stacking. For types smaller than a 
 * `size_t`, this is just going to be the identity function.
 */
size_t idhash(char *str, size_t len);