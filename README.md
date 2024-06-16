# Associative Array Testing/Benchmarking Suite

A variety of benchmarks and tests for **associative arrays**, also
called **dictionaries**, **maps** or **(symbol) tables**.

You can use it to benchmark C or C++ associative arrays, or any other
language that allows you to expose `extern "C"` functions, such as Rust.
The benchmarks suite only accepts **(integer ⇒ integer)** mappings.
Specifically, mappings from `keyint_t` to `valint_t` as defined in
`astv_array_vtable.h`. Currently, they are both defined as `intptr_t`,
which allows them to be used as pointers to objects managed by the
collection, for example.

## Usage

You will need to construct a `astv_array_vtable` for your 
implementation. This defines the operations listed on the wikipedia 
article, that is `insert()`, `remove()` and `lookup()`.