# Data structures and algorithms in C

This repository contains useful data structures written in C to allow for easy
reuse.

## Structure of repository

### Git branches

The `main` branch of this repository contains all the source code (for the data
structures and algorithms) laid out in the root of the repository for easy reuse
(through something like git submodules). The `dev` branch contains the same
source code (with possible new and unfinished data structures and algorithms),
but nested in a `src/` directory, along with a `Makefile` for compilation and
testing. All test files are located in the `test/` directory.

### Source code layout

All the source code files are grouped into directories with other
similar/necessary source code files. For example, all the collection data
structures (like `HashMap` and `LinkedList`) are stored in the `collection/`
directory.

## Library usage

This library is meant to be used just like the standard C library (`libc`) by
including the header files as needed. Unfortunately, you will also need to
include the necessary files for compilation. The intended way of using this
library is to clone the `main` branch of this repository (using git submodules
is advised) into the source directory of a project. Then one can simply provide
the necessary `#include` directives in their code, e.g.
`#include "stc/collection/hashmap.h"` to include the `HashMap` header, and then
include the necessary linking in their build system.

### Single header file: `api.h`

For convenience, there is a header file (`api.h`) that includes all the other
header files in the library. This allows the use of a single `#include`
directive to use all the data structures and algorithms, with a simple
`#include "stc/api.h"`.

### Macro feature flags

Almost all features in the library are enabled by default, but many features can
be disabled (opt-out) by defining certain macros. This can be helpful for
various reasons, such as not wanted to use the standard C library `math.h` that
needs to be linked (by defining the `STC_DISABLE_STD_MATH_H` macro). These
macros simply need to be defined before either (1) including the header file, or
(2) linking the `.c` file (such is the case for disabling the standard `math.h`
header). Most, if not all, of these opt-out macros contain `DISABLE` in them.
Each data structure and algorithm can specify their own opt-out macros, and you
simply need to look at the source code (mainly the header files).

### Using short names for function and struct names

By default, all the function and struct names are prefixed with `stc_` and
`Stc`, respectively. However, aliases (shorter names that strip the mentioned
prefixes) for these functions and structs can be enabled by defining the
`STC_*_ENABLE_SHORT_NAMES` for the specific data structure or algorithm you
want, e.g. `#define STC_UTF_ENABLE_SHORT_NAMES`. There is also a complete
`STC_ENABLE_SHORT_NAMES` which will enable the aliases for all functions and
structs.

### Macros in `api.h`

Since `api.h` includes all the other header files, there is an opt-out macro for
each data structure and algorithm, as well as "grouped" macros to disable entire
directories, such as `STC_DISABLE_COLLECTION` to opt-out of all collection data
structures. Note that some of these macros will affect other header files where
functions are defined that use other data structures or algorithms.
