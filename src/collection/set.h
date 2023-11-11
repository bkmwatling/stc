#ifndef STC_SET_H
#define STC_SET_H

#include <stddef.h>

typedef size_t stc_set_len_func(void *set_impl);
typedef int    stc_set_insert_func(void *set_impl, void *key);
typedef int    stc_set_contains_func(void *set_impl, void *key);
typedef void   stc_set_free_func(void *);
typedef void
stc_set_remove_func(void *set_impl, void *key, stc_set_free_func *keyfree);

typedef void **stc_set_keys_func(void *set_impl);
typedef void stc_set_free_set_func(void *set_impl, stc_set_free_func *keyfree);

typedef struct {
    stc_set_len_func      *len;
    stc_set_insert_func   *insert;
    stc_set_contains_func *contains;
    stc_set_remove_func   *remove;
    stc_set_keys_func     *keys;
    stc_set_free_set_func *free;

    void *impl;
} StcSet;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_SET_ENABLE_SHORT_NAMES)
typedef StcSet Set;

typedef stc_set_len_func      set_len_func;
typedef stc_set_insert_func   set_insert_func;
typedef stc_set_contains_func set_contains_func;
typedef stc_set_free_func     set_free_func;
typedef stc_set_remove_func   set_remove_func;

typedef stc_set_keys_func     set_keys_func;
typedef stc_set_free_set_func set_free_set_func;

#    define set_len        stc_set_len
#    define set_len_unsafe stc_set_len_unsafe
#    define set_is_empty   stc_set_is_empty

#    define set_insert   stc_set_insert
#    define set_contains stc_set_contains
#    define set_remove   stc_set_remove

#    define set_keys stc_set_keys
#    define set_free stc_set_free
#endif /* STC_SET_ENABLE_SHORT_NAMES */

#define stc_set_len(s)        ((s) ? stc_set_len_unsafe(m) : 0)
#define stc_set_len_unsafe(s) ((s)->len((s)->impl))
#define stc_set_is_empty(s)   (stc_set_len(s) == 0)

#define stc_set_insert(s, key, val) ((s)->insert((s)->impl, (key), (val)))
#define stc_set_contains(s, key)    ((s)->contains((s)->impl, (key)))
#define stc_set_remove(s, key, keyfree) \
    ((s)->remove((s)->impl, (key), (keyfree)))

#define stc_set_keys(s) ((s)->keys((s)->impl))
#define stc_set_free(s, keyfree, valfree) \
    ((s)->free((s)->impl, (keyfree), (valfree)))

/* Macro to easily generate as_set and to_set functions that wrap into StcSet */
#define STC_SET_CREATE_WRAPPER(prefix, settype)                               \
    static size_t prefix##_len_wrapper(void *set_impl)                        \
    {                                                                         \
        return prefix##_len((settype *) set_impl);                            \
    }                                                                         \
                                                                              \
    static int prefix##_insert_wrapper(void *set_impl, void *key)             \
    {                                                                         \
        return prefix##_insert((settype *) set_impl, key);                    \
    }                                                                         \
                                                                              \
    static int prefix##_contains_wrapper(void *set_impl, void *key)           \
    {                                                                         \
        return prefix##_contains((settype *) set_impl, key);                  \
    }                                                                         \
                                                                              \
    static void prefix##_remove_wrapper(void *set_impl, void *key,            \
                                        stc_set_free_func *keyfree)           \
    {                                                                         \
        prefix##_remove((settype *) set_impl, key, keyfree);                  \
    }                                                                         \
                                                                              \
    static void **prefix##_keys_wrapper(void *set_impl)                       \
    {                                                                         \
        return prefix##_keys((settype *) set_impl);                           \
    }                                                                         \
                                                                              \
    static void prefix##_free_wrapper(void              *set_impl,            \
                                      stc_set_free_func *keyfree)             \
    {                                                                         \
        prefix##_free((settype *) set_impl, keyfree);                         \
    }                                                                         \
                                                                              \
    static void stc_set_free_noop(void *set_impl, stc_set_free_func *keyfree) \
    {                                                                         \
        (void) set_impl;                                                      \
        (void) keyfree;                                                       \
    }                                                                         \
                                                                              \
    StcSet *prefix##_as_set(settype *set_impl)                                \
    {                                                                         \
        StcSet *s = malloc(sizeof(StcSet));                                   \
                                                                              \
        s->len      = prefix##_len_wrapper;                                   \
        s->insert   = prefix##_insert_wrapper;                                \
        s->contains = prefix##_contains_wrapper;                              \
        s->remove   = prefix##_remove_wrapper;                                \
        s->keys     = prefix##_keys_wrapper;                                  \
        s->free     = stc_set_free_noop;                                      \
        s->impl     = set_impl;                                               \
                                                                              \
        return s;                                                             \
    }                                                                         \
                                                                              \
    StcSet *prefix##_to_set(settype *set_impl)                                \
    {                                                                         \
        StcSet *s = malloc(sizeof(StcSet));                                   \
                                                                              \
        s->len      = prefix##_len_wrapper;                                   \
        s->insert   = prefix##_insert_wrapper;                                \
        s->contains = prefix##_contains_wrapper;                              \
        s->remove   = prefix##_remove_wrapper;                                \
        s->keys     = prefix##_keys_wrapper;                                  \
        s->free     = prefix##_free_wrapper;                                  \
        s->impl     = set_impl;                                               \
                                                                              \
        return s;                                                             \
    }

#endif /* STC_SET_H */
