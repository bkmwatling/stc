#ifndef STC_SET_H
#define STC_SET_H

#include <stdbool.h>
#include <stddef.h>

typedef size_t stc_set_len_func(void *set_impl);
typedef int    stc_set_insert_func(void *set_impl, void *elem);
typedef bool   stc_set_contains_func(void *set_impl, void *elem);
typedef void   stc_set_free_func(void *);
typedef void
stc_set_remove_func(void *set_impl, void *elem, stc_set_free_func *elemfree);

typedef void **stc_set_elems_func(void *set_impl);
typedef void stc_set_free_set_func(void *set_impl, stc_set_free_func *elemfree);

typedef struct {
    stc_set_len_func      *len;
    stc_set_insert_func   *insert;
    stc_set_contains_func *contains;
    stc_set_remove_func   *remove;
    stc_set_elems_func    *elems;
    stc_set_free_set_func *free;

    void *impl;
} StcSet;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_SET_ENABLE_SHORT_NAMES)
typedef StcSet Set;

#    define SET_ELEM_EXISTS STC_SET_ELEM_EXISTS
#    define SET_SUCCESS     STC_SET_SUCCESS

typedef stc_set_len_func      set_len_func;
typedef stc_set_insert_func   set_insert_func;
typedef stc_set_contains_func set_contains_func;
typedef stc_set_free_func     set_free_func;
typedef stc_set_remove_func   set_remove_func;

typedef stc_set_elems_func    set_elems_func;
typedef stc_set_free_set_func set_free_set_func;

#    define set_len        stc_set_len
#    define set_len_unsafe stc_set_len_unsafe
#    define set_is_empty   stc_set_is_empty

#    define set_insert   stc_set_insert
#    define set_contains stc_set_contains
#    define set_remove   stc_set_remove

#    define set_elems stc_set_elems
#    define set_free  stc_set_free
#endif /* STC_SET_ENABLE_SHORT_NAMES */

#define STC_SET_ELEM_EXISTS -1
#define STC_SET_SUCCESS     0

#define stc_set_len(s)        ((s) ? stc_set_len_unsafe(m) : 0)
#define stc_set_len_unsafe(s) ((s)->len((s)->impl))
#define stc_set_is_empty(s)   (stc_set_len(s) == 0)

#define stc_set_insert(s, elem, val) ((s)->insert((s)->impl, (elem), (val)))
#define stc_set_contains(s, elem)    ((s)->contains((s)->impl, (elem)))
#define stc_set_remove(s, elem, elemfree) \
    ((s)->remove((s)->impl, (elem), (elemfree)))

#define stc_set_elems(s)          ((s)->elems((s)->impl))
#define stc_set_free(s, elemfree) ((s)->free((s)->impl, (elemfree)))

/* Macro to easily generate as_set and to_set functions that wrap into StcSet */
#define STC_SET_CREATE_WRAPPER(prefix, settype)                                \
    static size_t prefix##_len_wrapper(void *set_impl)                         \
    {                                                                          \
        return prefix##_len((settype *) set_impl);                             \
    }                                                                          \
                                                                               \
    static int prefix##_insert_wrapper(void *set_impl, void *elem)             \
    {                                                                          \
        return prefix##_insert((settype *) set_impl, elem);                    \
    }                                                                          \
                                                                               \
    static bool prefix##_contains_wrapper(void *set_impl, void *elem)          \
    {                                                                          \
        return prefix##_contains((settype *) set_impl, elem);                  \
    }                                                                          \
                                                                               \
    static void prefix##_remove_wrapper(void *set_impl, void *elem,            \
                                        stc_set_free_func *elemfree)           \
    {                                                                          \
        prefix##_remove((settype *) set_impl, elem, elemfree);                 \
    }                                                                          \
                                                                               \
    static void **prefix##_elems_wrapper(void *set_impl)                       \
    {                                                                          \
        return prefix##_elems((settype *) set_impl);                           \
    }                                                                          \
                                                                               \
    static void prefix##_free_wrapper(void              *set_impl,             \
                                      stc_set_free_func *elemfree)             \
    {                                                                          \
        prefix##_free((settype *) set_impl, elemfree);                         \
    }                                                                          \
                                                                               \
    static void stc_set_free_noop(void *set_impl, stc_set_free_func *elemfree) \
    {                                                                          \
        (void) set_impl;                                                       \
        (void) elemfree;                                                       \
    }                                                                          \
                                                                               \
    StcSet *prefix##_as_set(settype *set_impl)                                 \
    {                                                                          \
        StcSet *s = malloc(sizeof(StcSet));                                    \
                                                                               \
        s->len      = prefix##_len_wrapper;                                    \
        s->insert   = prefix##_insert_wrapper;                                 \
        s->contains = prefix##_contains_wrapper;                               \
        s->remove   = prefix##_remove_wrapper;                                 \
        s->elems    = prefix##_elems_wrapper;                                  \
        s->free     = stc_set_free_noop;                                       \
        s->impl     = set_impl;                                                \
                                                                               \
        return s;                                                              \
    }                                                                          \
                                                                               \
    StcSet *prefix##_to_set(settype *set_impl)                                 \
    {                                                                          \
        StcSet *s = malloc(sizeof(StcSet));                                    \
                                                                               \
        s->len      = prefix##_len_wrapper;                                    \
        s->insert   = prefix##_insert_wrapper;                                 \
        s->contains = prefix##_contains_wrapper;                               \
        s->remove   = prefix##_remove_wrapper;                                 \
        s->elems    = prefix##_elems_wrapper;                                  \
        s->free     = prefix##_free_wrapper;                                   \
        s->impl     = set_impl;                                                \
                                                                               \
        return s;                                                              \
    }

#endif /* STC_SET_H */
