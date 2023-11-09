#ifndef STC_MAP_H
#define STC_MAP_H

#include <stddef.h>

typedef size_t stc_map_len_func(void *map);
typedef int    stc_map_insert_func(void *map, void *key, void *val);
typedef void  *stc_map_keyval_func(void *map, void *key);
typedef int    stc_map_contains_key_func(void *map, void *key);

typedef void **stc_map_keys_func(void *map);
typedef void **stc_map_values_func(void *map);

typedef void stc_map_free_func(void *);
typedef void stc_map_free_map_func(void              *map,
                                   stc_map_free_func *keyfree,
                                   stc_map_free_func *valfree);

typedef struct {
    stc_map_len_func          *len;
    stc_map_insert_func       *insert;
    stc_map_keyval_func       *get;
    stc_map_contains_key_func *contains_key;
    stc_map_keyval_func       *remove;
    stc_map_keys_func         *keys;
    stc_map_values_func       *values;
    stc_map_free_map_func     *free;
    void                      *map;
} StcMap;

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_MAP_ENABLE_SHORT_NAMES)
typedef StcMap Map;

typedef stc_map_len_func          map_len_func;
typedef stc_map_insert_func       map_insert_func;
typedef stc_map_keyval_func       map_keyval_func;
typedef stc_map_contains_key_func map_contains_key_func;

typedef stc_map_keys_func   map_keys_func;
typedef stc_map_values_func map_values_func;

typedef stc_map_free_func     map_free_func;
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

#define stc_map_len(m)        ((m) ? stc_map_len_unsafe(m) : 0)
#define stc_map_len_unsafe(m) ((m)->len((m)->map))
#define stc_map_is_empty(m)   (stc_map_len(m) == 0)

#define stc_map_insert(m, key, val)  ((m)->insert((m)->map, (key), (val)))
#define stc_map_get(m, key)          ((m)->get((m)->map, (key)))
#define stc_map_contains_key(m, key) ((m)->contains_key((m)->map, (key)))
#define stc_map_remove(m, key)       ((m)->remove((m)->map, (key)))

#define stc_map_keys(m)   ((m)->keys((m)->map))
#define stc_map_values(m) ((m)->values((m)->map))

#define stc_map_free(m, keyfree, valfree) \
    ((m)->free((m)->map, (keyfree), (valfree)))

/* Macro to easily generate as_map and to_map functions that wrap into StcMap */
#define STC_MAP_CREATE_WRAPPER(prefix, maptype)                              \
    static size_t prefix##_len_wrapper(void *map)                            \
    {                                                                        \
        return prefix##_len((maptype *) map);                                \
    }                                                                        \
                                                                             \
    static int prefix##_insert_wrapper(void *map, void *key, void *val)      \
    {                                                                        \
        return prefix##_insert((maptype *) map, key, val);                   \
    }                                                                        \
                                                                             \
    static void *prefix##_get_wrapper(void *map, void *key)                  \
    {                                                                        \
        return prefix##_get((maptype *) map, key);                           \
    }                                                                        \
                                                                             \
    static int prefix##_contains_key_wrapper(void *map, void *key)           \
    {                                                                        \
        return prefix##_contains_key((maptype *) map, key);                  \
    }                                                                        \
                                                                             \
    static void *prefix##_remove_wrapper(void *map, void *key)               \
    {                                                                        \
        return prefix##_remove((maptype *) map, key);                        \
    }                                                                        \
                                                                             \
    static void **prefix##_keys_wrapper(void *map)                           \
    {                                                                        \
        return prefix##_keys((maptype *) map);                               \
    }                                                                        \
                                                                             \
    static void **prefix##_values_wrapper(void *map)                         \
    {                                                                        \
        return prefix##_values((maptype *) map);                             \
    }                                                                        \
                                                                             \
    static void prefix##_free_wrapper(void *map, stc_map_free_func *keyfree, \
                                      stc_map_free_func *valfree)            \
    {                                                                        \
        prefix##_free((maptype *) map, keyfree, valfree);                    \
    }                                                                        \
                                                                             \
    static void stc_map_free_noop(void *map, stc_map_free_func *keyfree,     \
                                  stc_map_free_func *valfree)                \
    {                                                                        \
        (void) map;                                                          \
        (void) keyfree;                                                      \
        (void) valfree;                                                      \
    }                                                                        \
                                                                             \
    StcMap *prefix##_as_map(maptype *map)                                    \
    {                                                                        \
        StcMap *m = malloc(sizeof(StcMap));                                  \
                                                                             \
        m->len          = prefix##_len_wrapper;                              \
        m->insert       = prefix##_insert_wrapper;                           \
        m->get          = prefix##_get_wrapper;                              \
        m->contains_key = prefix##_contains_key_wrapper;                     \
        m->remove       = prefix##_remove_wrapper;                           \
        m->keys         = prefix##_keys_wrapper;                             \
        m->values       = prefix##_values_wrapper;                           \
        m->free         = stc_map_free_noop;                                 \
        m->map          = map;                                               \
                                                                             \
        return m;                                                            \
    }                                                                        \
                                                                             \
    StcMap *prefix##_to_map(maptype *map)                                    \
    {                                                                        \
        StcMap *m = malloc(sizeof(StcMap));                                  \
                                                                             \
        m->len          = prefix##_len_wrapper;                              \
        m->insert       = prefix##_insert_wrapper;                           \
        m->get          = prefix##_get_wrapper;                              \
        m->contains_key = prefix##_contains_key_wrapper;                     \
        m->remove       = prefix##_remove_wrapper;                           \
        m->keys         = prefix##_keys_wrapper;                             \
        m->values       = prefix##_values_wrapper;                           \
        m->free         = prefix##_free_wrapper;                             \
        m->map          = map;                                               \
                                                                             \
        return m;                                                            \
    }

#endif /* STC_MAP_H */
