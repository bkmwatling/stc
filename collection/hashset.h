#ifndef STC_HASHSET_H
#define STC_HASHSET_H

#include <stddef.h>

#if defined(STC_DISABLE_SET) && !defined(STC_HASHSET_DISABLE_SET)
#    define STC_HASHSET_DISABLE_SET
#endif

#ifndef STC_HASHSET_DISABLE_SET
#    include "set.h"
#endif

typedef struct stc_hashset StcHashSet;

typedef size_t stc_hashset_hash_func(void *key, size_t hashset_cap);
typedef int    stc_hashset_keycmp_func(void *key1, void *key2);
typedef void   stc_hashset_keyfree_func(void *);

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_HASHSET_ENABLE_SHORT_NAMES)
typedef StcHashSet HashSet;

typedef stc_hashset_hash_func    hashset_hash_func;
typedef stc_hashset_keycmp_func  hashset_keycmp_func;
typedef stc_hashset_keyfree_func hashset_keyfree_func;

#    define HASHSET_DEFAULT_CAPACITY   STC_HASHSET_DEFAULT_CAPACITY
#    define HASHSET_DEFAULT_LOADFACTOR STC_HASHSET_DEFAULT_LOADFACTOR

#    define HASHSET_OUT_OF_MEMORY STC_HASHSET_OUT_OF_MEMORY
#    define HASHSET_KEY_EXISTS    STC_HASHSET_KEY_EXISTS
#    define HASHSET_SUCCESS       STC_HASHSET_SUCCESS
#    define HASHSET_NULL_ARGUMENT STC_HASHSET_NULL_ARGUMENT

#    define hashset_new                   stc_hashset_new
#    define hashset_new_with_capacity     stc_hashset_new_with_capacity
#    define hashset_default               stc_hashset_default
#    define hashset_default_with_capacity stc_hashset_default_with_capacity

#    define hashset_len      stc_hashset_len
#    define hashset_is_empty stc_hashset_is_empty

#    define hashset_insert   stc_hashset_insert
#    define hashset_contains stc_hashset_contains
#    define hashset_remove   stc_hashset_remove
#    define hashset_keys     stc_hashset_keys
#    define hashset_free     stc_hashset_free

#    ifndef STC_HASHSET_DISABLE_SET
#        define hashset_as_set stc_hashset_as_set
#        define hashset_to_set stc_hashset_to_set
#    endif /* STC_HASHSET_DISABLE_SET */
#endif     /* STC_HASHSET_ENABLE_SHORT_NAMES */

#define STC_HASHSET_DEFAULT_CAPACITY   13
#define STC_HASHSET_DEFAULT_LOADFACTOR 0.75f

#define STC_HASHSET_OUT_OF_MEMORY -2
#define STC_HASHSET_KEY_EXISTS    -1
#define STC_HASHSET_SUCCESS       0
#define STC_HASHSET_NULL_ARGUMENT 1

#define stc_hashset_new(loadfactor, hash, keycmp)                             \
    stc_hashset_new_with_capacity(STC_HASHSET_DEFAULT_CAPACITY, (loadfactor), \
                                  (hash), (keycmp))
#define stc_hashset_default(hash, keycmp) \
    stc_hashset_new(STC_HASHSET_DEFAULT_LOADFACTOR, (hash), (keycmp))
#define stc_hashset_default_with_capacity(cap, hash, keycmp)             \
    stc_hashset_new_with_capacity((cap), STC_HASHSET_DEFAULT_LOADFACTOR, \
                                  (hash), (keycmp))
#define stc_hashset_is_empty(hashset) (stc_hashset_len(hashset) == 0)

/**
 * Creates a hash set with initial capacity of cap and maximum loadfactor of
 * loadfactor using the hash function hash and key comparison function
 * keycmp.
 *
 * @param[in] cap the initial capacity of the hash set
 * @param[in] loadfactor the maximum loadfactor of the hash set
 * @param[in] hash the hash function to use
 * @param[in] keycmp the key comparison function to use
 *
 * @return a pointer to the created hash set if all arguments are valid;
 *         else NULL if loadfactor <= 0.0f, hash is NULL, or keycmp is NULL
 */
StcHashSet *stc_hashset_new_with_capacity(size_t                   cap,
                                          float                    loadfactor,
                                          stc_hashset_hash_func   *hash,
                                          stc_hashset_keycmp_func *keycmp);

/**
 * Gets the length of (number of entries in) the hash set.
 *
 * @param[in] self the pointer to the hash set
 *
 * @return the length of the hash set if it is not NULL; else 0
 */
size_t stc_hashset_len(StcHashSet *self);

/**
 * Inserts the key into the hash set only if the key is not already in the hash
 * set.
 *
 * @param[in] self the pointer to the hash set
 * @param[in] key the key to insert into the hash set
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_hashset_insert(StcHashSet *self, void *key);

/**
 * Checks whether the key is contained in the hash set.
 *
 * @param[in] self the pointer to the hash set
 * @param[in] key the key to check for in the hash set
 *
 * @return non-zero value if the key was found; else 0 if it was not found
 */
int stc_hashset_contains(StcHashSet *self, void *key);

/**
 * Removes the key from the hash set if the key is contained in the hash set.
 *
 * @param[in] self the pointer to the hash set
 * @param[in] key the key to remove from the hash set
 * @param[in] keyfree the function used to free the key in the hash set if found
 */
void stc_hashset_remove(StcHashSet               *self,
                        void                     *key,
                        stc_hashset_keyfree_func *keyfree);

/**
 * Creates an unordered array of the keys contained in the hash set.
 *
 * @param[in] self the pointer to the hash set
 *
 * @return an unordered array of the keys contained in the hash set with the
 *         same length as the hash set
 */
void **stc_hashset_keys(StcHashSet *self);

/**
 * Frees the memory occupied by the hash set, freeing the memory of each key in
 * the hash set.
 *
 * @param[in] self the pointer to the hash set
 * @param[in] keyfree the function used to free each key in the hash set
 */
void stc_hashset_free(StcHashSet *self, stc_hashset_keyfree_func *keyfree);

#ifndef STC_HASHSET_DISABLE_SET
/**
 * Wraps the hash set inside a set "interface" but as a borrowed reference,
 * i.e., it cannot be freed from the set wrapper.
 *
 * @param[in] self the pointer to the hash set
 *
 * @return a pointer to the set "interface" that wraps around the hash set
 */
StcSet *stc_hashset_as_set(StcHashSet *self);

/**
 * Wraps the hash set inside a set "interface" giving full ownership to the set,
 * i.e., it can and must be freed from the set wrapper.
 *
 * @param[in] self the pointer to the hash set
 *
 * @return a pointer to the set "interface" that wraps around the hash set
 */
StcSet *stc_hashset_to_set(StcHashSet *self);
#endif /* STC_HASHSET_DISABLE_SET */

#endif /* STC_HASHSET_H */
