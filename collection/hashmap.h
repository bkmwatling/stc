#ifndef STC_HASHMAP_H
#define STC_HASHMAP_H

#include <stddef.h>

#if defined(STC_DISABLE_MAP) && !defined(STC_HASHMAP_DISABLE_MAP)
#    define STC_HASHMAP_DISABLE_MAP
#endif

#ifndef STC_HASHMAP_DISABLE_MAP
#    include "map.h"
#endif

typedef struct stc_hashmap StcHashMap;

typedef size_t stc_hashmap_hash_func(void *key, size_t hashmap_cap);
typedef int    stc_hashmap_keycmp_func(void *key1, void *key2);
typedef void   stc_hashmap_keyval_free_func(void *);

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_HASHMAP_ENABLE_SHORT_NAMES)
typedef StcHashMap HashMap;

typedef stc_hashmap_hash_func        hashmap_hash_func;
typedef stc_hashmap_keycmp_func      hashmap_keycmp_func;
typedef stc_hashmap_keyval_free_func hashmap_keyval_free_func;

#    define HASHMAP_DEFAULT_CAPACITY   STC_HASHMAP_DEFAULT_CAPACITY
#    define HASHMAP_DEFAULT_LOADFACTOR STC_HASHMAP_DEFAULT_LOADFACTOR

#    define HASHMAP_OUT_OF_MEMORY         STC_HASHMAP_OUT_OF_MEMORY
#    define HASHMAP_KEY_VALUE_PAIR_EXISTS STC_HASHMAP_KEY_VALUE_PAIR_EXISTS
#    define HASHMAP_SUCCESS               STC_HASHMAP_SUCCESS
#    define HASHMAP_NULL_ARGUMENT         STC_HASHMAP_NULL_ARGUMENT

#    define hashmap_new                   stc_hashmap_new
#    define hashmap_new_with_capacity     stc_hashmap_new_with_capacity
#    define hashmap_default               stc_hashmap_default
#    define hashmap_default_with_capacity stc_hashmap_default_with_capacity

#    define hashmap_len      stc_hashmap_len
#    define hashmap_is_empty stc_hashmap_is_empty

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

#define STC_HASHMAP_DEFAULT_CAPACITY   13
#define STC_HASHMAP_DEFAULT_LOADFACTOR 0.75f

#define STC_HASHMAP_OUT_OF_MEMORY         -2
#define STC_HASHMAP_KEY_VALUE_PAIR_EXISTS -1
#define STC_HASHMAP_SUCCESS               0
#define STC_HASHMAP_NULL_ARGUMENT         1

#define stc_hashmap_new(loadfactor, hash, keycmp)                             \
    stc_hashmap_new_with_capacity(STC_HASHMAP_DEFAULT_CAPACITY, (loadfactor), \
                                  (hash), (keycmp))
#define stc_hashmap_default(hash, keycmp) \
    stc_hashmap_new(STC_HASHMAP_DEFAULT_LOADFACTOR, (hash), (keycmp))
#define stc_hashmap_default_with_capacity(cap, hash, keycmp)             \
    stc_hashmap_new_with_capacity((cap), STC_HASHMAP_DEFAULT_LOADFACTOR, \
                                  (hash), (keycmp))
#define stc_hashmap_is_empty(hashmap) (stc_hashmap_len(hashmap) == 0)

/**
 * Creates a hash map with initial capacity of cap and maximum loadfactor of
 * loadfactor using the hash function hash and key comparison function
 * keycmp.
 *
 * @param[in] cap        the initial capacity of the hash map
 * @param[in] loadfactor the maximum loadfactor of the hash map
 * @param[in] hash       the hash function to use
 * @param[in] keycmp     the key comparison function to use
 *
 * @return a pointer to the created hash map if all arguments are valid;
 *         else NULL if loadfactor <= 0.0f, hash is NULL, or keycmp is NULL
 */
StcHashMap *stc_hashmap_new_with_capacity(size_t                   cap,
                                          float                    loadfactor,
                                          stc_hashmap_hash_func   *hash,
                                          stc_hashmap_keycmp_func *keycmp);

/**
 * Gets the length of (number of entries in) the hash map.
 *
 * @param[in] self the pointer to the hash map
 *
 * @return the length of the hash map if it is not NULL; else 0
 */
size_t stc_hashmap_len(StcHashMap *self);

/**
 * Inserts the key and value into the hash map only if the key is not already in
 * the hash map.
 *
 * @param[in] self the pointer to the hash map
 * @param[in] key  the key to insert into the hash map
 * @param[in] val  the value to insert into the hash map
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_hashmap_insert(StcHashMap *self, void *key, void *val);

/**
 * Gets the value corresponding to the key from the hash map if the key is
 * contained in the hash map.
 *
 * @param[in] self the pointer to the hash map
 * @param[in] key  the key to get the corresponding value of from the hash map
 *
 * @return the value corresponding to the key if found; else NULL
 */
void *stc_hashmap_get(StcHashMap *self, void *key);

/**
 * Checks whether the key is contained in the hash map.
 *
 * @param[in] self the pointer to the hash map
 * @param[in] key  the key to check for in the hash map
 *
 * @return non-zero value if the key was found; else 0 if it was not found
 */
int stc_hashmap_contains_key(StcHashMap *self, void *key);

/**
 * Removes the key-value pair from the hash map if the key is contained in the
 * hash map and returns the value corresponding to the key.
 *
 * @param[in] self    the pointer to the hash map
 * @param[in] key     the key of the key-value pair to remove from the hash map
 * @param[in] keyfree the function used to free the key in the hash map if found
 *
 * @return the value corresponding to the key if the key is contained in the
 *         hash map; else NULL if it is not
 */
void *stc_hashmap_remove(StcHashMap                   *self,
                         void                         *key,
                         stc_hashmap_keyval_free_func *keyfree);

/**
 * Creates an unordered array of the keys contained in the hash map.
 *
 * @param[in] self the pointer to the hash map
 *
 * @return an unordered array of the keys contained in the hash map with the
 *         same length as the hash map
 */
void **stc_hashmap_keys(StcHashMap *self);

/**
 * Creates an unordered array of the values contained in the hash map.
 *
 * @param[in] self the pointer to the hash map
 *
 * @return an array of the values contained in the hash map in the order
 *         corresponding to a call to stc_hashmap_keys with the same length as
 *         the hash map
 */
void **stc_hashmap_values(StcHashMap *self);

/**
 * Frees the memory occupied by the hash map, freeing the memory of each key and
 * value in the hash map.
 *
 * @param[in] self    the pointer to the hash map
 * @param[in] keyfree the function used to free each key in the hash map
 * @param[in] valfree the function used to free each value in the hash map
 */
void stc_hashmap_free(StcHashMap                   *self,
                      stc_hashmap_keyval_free_func *keyfree,
                      stc_hashmap_keyval_free_func *valfree);

#ifndef STC_HASHMAP_DISABLE_MAP
/**
 * Wraps the hash map inside a map "interface" but as a borrowed reference,
 * i.e., it cannot be freed from the map wrapper.
 *
 * @param[in] self the pointer to the hash map
 *
 * @return a pointer to the map "interface" that wraps around the hash map
 */
StcMap *stc_hashmap_as_map(StcHashMap *self);

/**
 * Wraps the hash map inside a map "interface" giving full ownership to the map,
 * i.e., it can and must be freed from the map wrapper.
 *
 * @param[in] self the pointer to the hash map
 *
 * @return a pointer to the map "interface" that wraps around the hash map
 */
StcMap *stc_hashmap_to_map(StcHashMap *self);
#endif /* STC_HASHMAP_DISABLE_MAP */

#endif /* STC_HASHMAP_H */
