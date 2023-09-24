#ifndef STC_HASHMAP_H
#define STC_HASHMAP_H

#ifndef STC_HASHMAP_DISABLE_MAP
#    include "map.h"
#endif

#ifdef STC_HASHMAP_ENABLE_SHORT_NAMES
typedef StcHashMap HashMap;

#    define hashmap_hash_func        stc_hashmap_hash_func
#    define hashmap_keycmp_func      stc_hashmap_keycmp_func
#    define hashmap_keyval_free_func stc_hashmap_keyval_free_func

#    define HASHMAP_DEFAULT_CAPACITY   STC_HASHMAP_DEFAULT_CAPACITY
#    define HASHMAP_DEFAULT_LOADFACTOR STC_HASHMAP_DEFAULT_LOADFACTOR

#    define hashmap_new                   stc_hashmap_new
#    define hashmap_new_with_capacity     stc_hashmap_new_with_capacity
#    define hashmap_default               stc_hashmap_default
#    define hashmap_default_with_capacity stc_hashmap_default_with_capacity

#    define hashmap_len          stc_hashmap_len
#    define hashmap_insert       stc_hashmap_insert
#    define hashmap_get          stc_hashmap_get
#    define hashmap_contains_key stc_hashmap_contains_key
#    define hashmap_remove       stc_hashmap_remove
#    define hashmap_keys         stc_hashmap_keys
#    define hashmap_values       stc_hashmap_values
#    define hashmap_free         stc_hashmap_free

#    ifndef STC_HASHMAP_DISABLE_MAP
#        define hashmap_as_map stc_hashmap_as_map
#        define hashmap_to_map stc_hashmap_to_map
#    endif /* STC_HASHMAP_DISABLE_MAP */
#endif     /* STC_HASHMAP_ENABLE_SHORT_NAMES */

typedef struct stc_hashmap StcHashMap;

typedef size_t stc_hashmap_hash_func(void *key, size_t hashmap_cap);
typedef int    stc_hashmap_keycmp_func(void *key1, void *key2);
typedef void   stc_hashmap_keyval_free_func(void *);

#define STC_HASHMAP_DEFAULT_CAPACITY   13
#define STC_HASHMAP_DEFAULT_LOADFACTOR 0.75f

#define stc_hashmap_new(loadfactor, hash, keycmp)                             \
    stc_hashmap_new_with_capacity(STC_HASHMAP_DEFAULT_CAPACITY, (loadfactor), \
                                  (hash), (keycmp))
#define stc_hashmap_default(hash, keycmp) \
    stc_hashmap_new(STC_HASHMAP_DEFAULT_LOADFACTOR, (hash), (keycmp))
#define stc_hashmap_default_with_capacity(cap, hash, keycmp)             \
    stc_hashmap_new_with_capacity((cap), STC_HASHMAP_DEFAULT_LOADFACTOR, \
                                  (hash), (keycmp))

StcHashMap *stc_hashmap_new_with_capacity(size_t                   cap,
                                          float                    loadfactor,
                                          stc_hashmap_hash_func   *hash,
                                          stc_hashmap_keycmp_func *keycmp);

size_t stc_hashmap_len(StcHashMap *hashmap);

int stc_hashmap_insert(StcHashMap *hashmap, void *key, void *val);

void *stc_hashmap_get(StcHashMap *hashmap, void *key);

int stc_hashmap_contains_key(StcHashMap *hashmap, void *key);

void *stc_hashmap_remove(StcHashMap *hashmap, void *key);

void **stc_hashmap_keys(StcHashMap *hashmap);

void **stc_hashmap_values(StcHashMap *hashmap);

void stc_hashmap_free(StcHashMap                   *hashmap,
                      stc_hashmap_keyval_free_func *keyfree,
                      stc_hashmap_keyval_free_func *valfree);

#ifndef STC_HASHMAP_DISABLE_MAP
StcMap *stc_hashmap_as_map(StcHashMap *hashmap);

StcMap *stc_hashmap_to_map(StcHashMap *hashmap);
#endif /* STC_HASHMAP_DISABLE_MAP */

#endif /* STC_HASHMAP_H */
