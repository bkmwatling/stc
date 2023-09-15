#ifndef VEC_H
#define VEC_H

#include <stddef.h>
#include <string.h>

typedef struct {
    size_t len;
    size_t cap;
} VecHeader;

#define VEC_DEFAULT_CAP 4

#define vec_header(v) ((VecHeader *) (v) -1)

#define vec_new(size, cap)  _vec_reserve_exact(NULL, (size), (cap))
#define vec_init(v, cap)    vec_reserve_exact(v, cap)
#define vec_default(size)   vec_new(size, VEC_DEFAULT_CAP)
#define vec_default_init(v) vec_init(v, VEC_DEFAULT_CAP)
#define vec_clone(v)        _vec_clone((v), sizeof(*(v)))
#define vec_free(v)         ((v) ? free(vec_header(v)) : (void) 0)

#define vec_len(v)          ((v) ? vec_header(v)->len : 0)
#define vec_cap(v)          ((v) ? vec_header(v)->cap : 0)
#define vec_is_empty(v)     (vec_len(v) == 0)
#define vec_clear(v)        ((v) ? vec_header(v)->len = 0 : (void) 0)
#define vec_push(v, x)      (vec_reserve(v, 1), (v)[vec_header(v)->len++] = (x))
#define vec_pop(v)          ((v) ? (v)[--(vec_header(v)->len)] : (void) 0)
#define vec_insert(v, i, x) (vec_reserve_index(v, i, 1), (v)[(i)] = (x))
#define vec_remove(v, i)                                    \
    ((v) ? (memmove((v) + (i), (v) + (i) + 1,               \
                    sizeof(*(v)) * (vec_len(v) - 1 - (i))), \
            vec_header(v)->len--)                           \
         : (void) 0)
#define vec_extend(v, slice, len)              \
    (assert(sizeof(*(v)) == sizeof(*(slice))), \
     (v) = _vec_extend((v), (slice), sizeof(*(slice)), (len)))
#define vec_reserve_exact(v, n) \
    ((v) = _vec_reserve_exact((v), sizeof(*(v)), (n)))
#define vec_reserve(v, n) ((v) = _vec_reserve((v), sizeof(*(v)), (n)))
#define vec_reserve_index(v, i, n)                          \
    (vec_reserve(v, n), memmove((v) + (i) + (n), (v) + (i), \
                                sizeof(*(v)) * (vec_len(v) - (n) - (i))))
#define vec_shrink(v, cap)   ((v) = _vec_shrink((v), sizeof(*(v)), (cap))
#define vec_shrink_to_fit(v) vec_shrink(v, vec_len(v))

/**
 * Clones (creates a copy of) a vector.
 *
 * @param[in] vec the vector to clone
 * @param[in] size the size of each element
 *
 * @return a clone of a vector
 */
void *_vec_clone(const void *vec, size_t size);

/**
 * Extends the vector by with the values from the slice.
 *
 * @param[in] vec pointer to the vector to extend
 * @param[in] slice the memory address of the slice to extend from
 * @param[in] size the size of each element
 * @param[in] len the length of the slice
 *
 * @return pointer to the vector after extended with slice
 */
void *_vec_extend(void *vec, const void *slice, size_t size, size_t len);

/**
 * Reserves exactly enough space in the vector for n more elements.
 *
 * @param[in] vec pointer to the vector to reserve space for
 * @param[in] size the size of each element
 * @param[in] n the number of elements to ensure space for
 *
 * @return pointer to the vector after reserving the space
 */
void *_vec_reserve_exact(void *vec, size_t size, size_t n);

/**
 * Reserves enough space in the vector for at least n more elements.
 *
 * @param[in] vec pointer to the vector to reserve space for
 * @param[in] size the size of each element
 * @param[in] n the number of elements to ensure space for
 *
 * @return pointer to the vector after reserving the space
 */
void *_vec_reserve(void *vec, size_t size, size_t n);

#endif /* VEC_H */
