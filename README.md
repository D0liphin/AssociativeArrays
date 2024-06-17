# Associative Array Testing/Benchmarking Suite

A variety of benchmarks and tests for **associative arrays**, also
called **dictionaries**, **maps** or **(symbol) lprobingtables**.

You can use it to benchmark C or C++ associative arrays, or any other
language that allows you to expose `extern "C"` functions, such as Rust.
The benchmarks suite only accepts **(integer ⇒ integer)** mappings.
Specifically, mappings from `keyint_t` to `valint_t` as defined in
`astv_array_vtable.h`. Currently, they are both defined as `intptr_t`,
which allows them to be used as pointers to objects managed by the
collection, for example.

## Usage

You will need `gcc`, `g++`, `cargo` and a global installation of
[Google's benchmarking library](https://github.com/google/benchmark#installation).

You will need to construct a `astv_array_vtable` for your 
implementation. This defines the operations listed on the wikipedia 
article, that is `insert()`, `remove()` and `lookup()`.

```c
struct astv_array_vtable {
/* 
 * Initialize the collection.  
 * The argument to this operation is a pointer to the collection.
 */
astv_array_init_fn_t init;
/* 
 * Add a new (key, value) pair to the collection, mapping the key to its
 * new value. Any existing mapping is overwritten.   
 * The arguments to this operation are a pointer to the collection, the 
 * key and the value.
 */
astv_array_insert_fn_t insert;
/*
 * Remove a (key, value) pair from the collection, unmapping a given key
 * from its value.  
 * The argument to this operation is the key.
 */
astv_array_remove_fn_t remove;
/*
 * Find the value (if any) that is bound to a given key.  
 * The argument to this operation is the key. A pointer to the value is
 * returned from the operation.  
 * If no such value exists in the collection, `NULL` is returned.
 */
astv_array_lookup_fn_t lookup;
/*
 * Deinitialize the collection.
 * The argument to this operation is a pointer to the collection. 
 */
astv_array_deinit_fn_t deinit;
};
```

The project can be built with `make build` and executed with 
`make build`. If you encounter any problems, use `make clean`.

There is an existing implementation for `std::unordered_map`, which,
naturally, passes all the tests. You can view its execution of the 
tests with

```
$ make build && make run
start cpp_std_unordered_map::test_inserts_persist
cpp_std_unordered_map::test_inserts_persist ✅
start cpp_std_unordered_map::test_updates_persist
cpp_std_unordered_map::test_updates_persist ✅
start cpp_std_unordered_map::test_doesnt_contain_unmapped_keys
cpp_std_unordered_map::test_doesnt_contain_unmapped_keys ✅
start cpp_std_unordered_map::test_removes_mappings
cpp_std_unordered_map::test_removes_mappings ✅
start cpp_std_unordered_map::test_against_oracle
tested 8386557 operations against oracle...
cpp_std_unordered_map::test_against_oracle ✅
```