#ifndef STC_MAP_H
#define STC_MAP_H

#include <stddef.h>

typedef size_t stc_map_len_func(void *map_impl);
typedef int    stc_map_insert_func(void *map_impl, void *key, void *val);
typedef void  *stc_map_get_func(void *map_impl, void *key);
typedef int    stc_map_contains_key_func(void *map_impl, void *key);
typedef void   stc_map_free_func(void *);
typedef void *
stc_map_remove_func(void *map_impl, void *key, stc_map_free_func *keyfree);

typedef void **stc_map_keys_func(void *map_impl);
typedef void **stc_map_values_func(void *map_impl);

typedef void stc_map_free_map_func(void              *map_impl,
                                   stc_map_free_func *keyfree,
                                   stc_map_free_func *valfree);

typedef struct {
    stc_map_len_func          *len;
    stc_map_insert_func       *insert;
    stc_map_get_func          *get;
    stc_map_contains_key_func *contains_key;
    stc_map_remove_func       *remove;
    stc_map_keys_func         *keys;
    stc_map_values_func       *values;
    stc_map_free_map_func     *free;

    void *impl;
} StcMap;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_MAP_ENABLE_SHORT_NAMES)
typedef StcMap Map;

#    define MAP_KEY_VALUE_PAIR_EXISTS STC_MAP_KEY_VALUE_PAIR_EXISTS
#    define MAP_SUCCESS               STC_MAP_SUCCESS

typedef stc_map_len_func          map_len_func;
typedef stc_map_insert_func       map_insert_func;
typedef stc_map_get_func          map_get_func;
typedef stc_map_contains_key_func map_contains_key_func;
typedef stc_map_free_func         map_free_func;
typedef stc_map_remove_func       map_remove_func;

typedef stc_map_keys_func   map_keys_func;
typedef stc_map_values_func map_values_func;

typedef stc_map_free_map_func map_free_map_func;

#    define map_len        stc_map_len
#    define map_len_unsafe stc_map_len_unsafe
#    define map_is_empty   stc_map_is_empty

#    define map_insert       stc_map_insert
#    define map_get          stc_map_get
#    define map_contains_key stc_map_contains_key
#    define map_remove       stc_map_remove

#    define map_keys   stc_map_keys
#    define map_values stc_map_values

#    define map_free stc_map_free
#endif /* STC_MAP_ENABLE_SHORT_NAMES */

#define STC_MAP_KEY_VALUE_PAIR_EXISTS -1
#define STC_MAP_SUCCESS               0

#define stc_map_len(m)        ((m) ? stc_map_len_unsafe(m) : 0)
#define stc_map_len_unsafe(m) ((m)->len((m)->impl))
#define stc_map_is_empty(m)   (stc_map_len(m) == 0)

#define stc_map_insert(m, key, val)  ((m)->insert((m)->impl, (key), (val)))
#define stc_map_get(m, key)          ((m)->get((m)->impl, (key)))
#define stc_map_contains_key(m, key) ((m)->contains_key((m)->impl, (key)))
#define stc_map_remove(m, key, keyfree) \
    ((m)->remove((m)->impl, (key), (keyfree)))

#define stc_map_keys(m)   ((m)->keys((m)->impl))
#define stc_map_values(m) ((m)->values((m)->impl))

#define stc_map_free(m, keyfree, valfree) \
    ((m)->free((m)->impl, (keyfree), (valfree)))

/* Macro to easily generate as_map and to_map functions that wrap into StcMap */
#define STC_MAP_CREATE_WRAPPER(prefix, maptype)                               \
    static size_t prefix##_len_wrapper(void *map_impl)                        \
    {                                                                         \
        return prefix##_len((maptype *) map_impl);                            \
    }                                                                         \
                                                                              \
    static int prefix##_insert_wrapper(void *map_impl, void *key, void *val)  \
    {                                                                         \
        return prefix##_insert((maptype *) map_impl, key, val);               \
    }                                                                         \
                                                                              \
    static void *prefix##_get_wrapper(void *map_impl, void *key)              \
    {                                                                         \
        return prefix##_get((maptype *) map_impl, key);                       \
    }                                                                         \
                                                                              \
    static int prefix##_contains_key_wrapper(void *map_impl, void *key)       \
    {                                                                         \
        return prefix##_contains_key((maptype *) map_impl, key);              \
    }                                                                         \
                                                                              \
    static void *prefix##_remove_wrapper(void *map_impl, void *key,           \
                                         stc_map_free_func *keyfree)          \
    {                                                                         \
        return prefix##_remove((maptype *) map_impl, key, keyfree);           \
    }                                                                         \
                                                                              \
    static void **prefix##_keys_wrapper(void *map_impl)                       \
    {                                                                         \
        return prefix##_keys((maptype *) map_impl);                           \
    }                                                                         \
                                                                              \
    static void **prefix##_values_wrapper(void *map_impl)                     \
    {                                                                         \
        return prefix##_values((maptype *) map_impl);                         \
    }                                                                         \
                                                                              \
    static void prefix##_free_wrapper(void              *map_impl,            \
                                      stc_map_free_func *keyfree,             \
                                      stc_map_free_func *valfree)             \
    {                                                                         \
        prefix##_free((maptype *) map_impl, keyfree, valfree);                \
    }                                                                         \
                                                                              \
    static void stc_map_free_noop(void *map_impl, stc_map_free_func *keyfree, \
                                  stc_map_free_func *valfree)                 \
    {                                                                         \
        (void) map_impl;                                                      \
        (void) keyfree;                                                       \
        (void) valfree;                                                       \
    }                                                                         \
                                                                              \
    StcMap *prefix##_as_map(maptype *map_impl)                                \
    {                                                                         \
        StcMap *m = malloc(sizeof(StcMap));                                   \
                                                                              \
        m->len          = prefix##_len_wrapper;                               \
        m->insert       = prefix##_insert_wrapper;                            \
        m->get          = prefix##_get_wrapper;                               \
        m->contains_key = prefix##_contains_key_wrapper;                      \
        m->remove       = prefix##_remove_wrapper;                            \
        m->keys         = prefix##_keys_wrapper;                              \
        m->values       = prefix##_values_wrapper;                            \
        m->free         = stc_map_free_noop;                                  \
        m->impl         = map_impl;                                           \
                                                                              \
        return m;                                                             \
    }                                                                         \
                                                                              \
    StcMap *prefix##_to_map(maptype *map_impl)                                \
    {                                                                         \
        StcMap *m = malloc(sizeof(StcMap));                                   \
                                                                              \
        m->len          = prefix##_len_wrapper;                               \
        m->insert       = prefix##_insert_wrapper;                            \
        m->get          = prefix##_get_wrapper;                               \
        m->contains_key = prefix##_contains_key_wrapper;                      \
        m->remove       = prefix##_remove_wrapper;                            \
        m->keys         = prefix##_keys_wrapper;                              \
        m->values       = prefix##_values_wrapper;                            \
        m->free         = prefix##_free_wrapper;                              \
        m->impl         = map_impl;                                           \
                                                                              \
        return m;                                                             \
    }

#endif /* STC_MAP_H */
