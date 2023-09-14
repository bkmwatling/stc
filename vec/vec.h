#ifndef VEC_H
#define VEC_H

#include <stddef.h>

#define VEC_DEFAULT_CAP 4

#define VEC_AT_PTR(vec, idx, type) (((type *) (vec).data) + (idx))
#define VEC_AT(vec, idx, type)     (*VEC_AT_PTR(vec, idx, type))

#define vec_free(vec)         free((vec).data)
#define vec_append(vec, item) _vec_append((vec), (const void *) &(item))
#define vec_append_literal(vec, item, type) \
    vec_reserve((vec), 1);                  \
    VEC_AT(*(vec), (vec)->len++, type) = (item)
#define vec_extend(vec, slice, len) \
    _vec_extend((vec), (const void *) (slice), (len))

typedef struct vec {
    void  *data; /* underlying data memory */
    size_t size; /* size of each element */
    size_t len;  /* number of elements (length) allocated */
    size_t cap;  /* total capacity of underlying memory */
} Vec;

/**
 * Creates a new Vec with elements size of size and capacity of cap.
 *
 * @param[in] size the size of each element in the Vec
 * @param[in] cap the initial capacity of the Vec
 *
 * @return a Vec with element size of size and capacity of cap
 */
Vec vec_new(size_t size, size_t cap);

/**
 * Creates a new Vec with elements size of size and default capacity.
 *
 * @param[in] size the size of each element in the Vec
 *
 * @return a Vec with element size of size and default capacity
 */
Vec vec_default(size_t size);

/**
 * Clones (creates a copy) a Vec.
 *
 * @param[in] vec the Vec to clone
 *
 * @return a clone of a Vec, creating a copy of the underlying memory
 */
Vec vec_clone(Vec vec);

/**
 * Appends value pointed to by item to the end of the Vec.
 *
 * @param[in] vec pointer to the Vec to append to
 * @param[in] item memory address of value to append
 *
 * @return non-zero if the append operation was successful; else 0
 */
int _vec_append(Vec *vec, const void *item);

/**
 * Extends the vector by with the values from the slice.
 *
 * @param[in] vec pointer to the Vec to extend
 * @param[in] slice the memory address of the slice to extend from
 * @param[in] len the length of the slice
 *
 * @return non-zero if the extend operation was successful; else 0
 */
int _vec_extend(Vec *vec, const void *slice, size_t len);

/**
 * Reserves exactly enough space in Vec for n more elements.
 *
 * @param[in] vec pointer to the Vec to reserve space for
 * @param[in] n the number of elements to ensure space for
 */
void vec_reserve_exact(Vec *vec, size_t n);

/**
 * Reserves enough space in Vec for at least n more elements.
 *
 * @param[in] vec pointer to the Vec to reserve space for
 * @param[in] n the number of elements to ensure space for
 */
void vec_reserve(Vec *vec, size_t n);

#endif /* VEC_H */
