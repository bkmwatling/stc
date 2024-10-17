# Data structures and algorithms in C

This repository contains useful data structures written in C to allow for easy
reuse.

## Structure of repository

### Git branches

The `main` branch of this repository contains all the source code (for the data
structures and algorithms) laid out in the `src/` directory of the repository,
along with a `Makefile` for compilation and testing. The `dev` branch contains
the same source code, but with possible new and unfinished data structures and
algorithms. All test files are located in the `test/` directory.

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
is advised) into your project directory/repository. During compilation, one just
needs to add the include flag `-I` with the path to the `include/` directory of
`stc` (e.g. `-I./stc/include` if you cloned the repository to the `stc/`
directory). Then one can simply provide the necessary `#include` directives in
their code, e.g. `#include <stc/collection/linkedlist.h>` to include the
`LinkedList` header, and then include the necessary linking in their build
system.

### Using the `clangd` Language Server Protocol

If you use `clangd` as an LSP for your editor, it may complain about not finding
the necessary header files. To remedy this, simply create a `.clangd` file in
the root of your project with the include compile flag as follows:

```
CompileFlags:
  Add:
    - -I/path/to/stc/include/directory
```

### Single header file: `api.h`

For convenience, there is a header file (`api.h`) that includes all the other
header files in the library. This allows the use of a single `#include`
directive to use all the data structures and algorithms, with a simple
`#include <stc/api.h>`. Please note that this header file is **not** a
standalone header file, but rather simply defers to all other necessary header
files for definitions.

### Macro feature flags

Almost all features in the library are enabled by default, but many features can
be disabled (opt-out) by defining certain macros. This can be helpful for
various reasons, such as not wanting to use the standard C library `math.h` that
needs to be linked (by defining the `STC_DISABLE_STD_MATH_H` macro). These
macros simply need to be defined before either (1) including the header file, or
(2) linking the `.c` file (such is the case for disabling the standard C library
`math.h` header). Most, if not all, of these opt-out macros start with
`STC_DISABLE`. Each data structure and algorithm can specify their own opt-out
macros, and you simply need to look at the source code (mainly the header
files).

### Using short names for function and struct names

By default, all the function and structure names are prefixed with `stc_` and
`Stc`, respectively. However, aliases (shorter names that strip the mentioned
prefixes) for these functions and structures can be enabled by defining the
`STC_*_ENABLE_SHORT_NAMES` for the specific data structure or algorithm you
want, e.g. `#define STC_UTF_ENABLE_SHORT_NAMES`. There is also a complete
`STC_ENABLE_SHORT_NAMES` which will enable the aliases for all functions and
structures.

### Macros in `api.h`

Since `api.h` includes all the other header files, there is an opt-out macro for
each data structure and algorithm, as well as "grouped" macros to disable entire
directories, such as `STC_DISABLE_COLLECTION` to opt-out of all collection data
structures. Note that some of these macros will affect other header files where
functions are defined that use other data structures or algorithms.
