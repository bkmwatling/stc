#ifndef STC_BTREE_H
#define STC_BTREE_H

#include <stddef.h>

#if defined(STC_DISABLE_MAP) && !defined(STC_BTREE_DISABLE_MAP)
#    define STC_BTREE_DISABLE_MAP
#endif

#ifndef STC_BTREE_DISABLE_MAP
#    include "map.h"
#endif

typedef struct stc_btree StcBTree;

typedef int  stc_btree_keycmp_func(void *key1, void *key2);
typedef void stc_btree_keyval_free_func(void *);

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_BTREE_ENABLE_SHORT_NAMES)
typedef StcBTree BTree;

typedef stc_btree_keycmp_func      stc_btree_keycmp_func;
typedef stc_btree_keyval_free_func stc_btree_keyval_free_func;

#    define BTREE_KEY_VALUE_PAIR_EXISTS STC_BTREE_KEY_VALUE_PAIR_EXISTS
#    define BTREE_SUCCESS               STC_BTREE_SUCCESS
#    define BTREE_NULL_ARGUMENT         STC_BTREE_NULL_ARGUMENT

#    define btree_new      stc_btree_new
#    define btree_len      stc_btree_len
#    define btree_is_empty stc_btree_is_empty

#    define btree_insert       stc_btree_insert
#    define btree_get          stc_btree_get
#    define btree_contains_key stc_btree_contains_key
#    define btree_remove       stc_btree_remove
#    define btree_keys         stc_btree_keys
#    define btree_values       stc_btree_values
#    define btree_free         stc_btree_free

#    ifndef STC_BTREE_DISABLE_MAP
#        define btree_as_map stc_btree_as_map
#        define btree_to_map stc_btree_to_map
#    endif /* STC_BTREE_DISABLE_MAP */
#endif     /* STC_BTREE_ENABLE_SHORT_NAMES */

#define STC_BTREE_KEY_VALUE_PAIR_EXISTS -1
#define STC_BTREE_SUCCESS               0
#define STC_BTREE_NULL_ARGUMENT         1

#define stc_btree_is_empty(btree) (stc_btree_len(btree) == 0)

/**
 * Create a BTree with key comparison function keycmp.
 *
 * @param[in] keycmp the key comparison function to use
 *
 * @return a pointer to the created BTree if keycmp is not NULL; else NULL
 */
StcBTree *stc_btree_new(stc_btree_keycmp_func *keycmp);

/**
 * Get the length of (number of entries in) the BTree.
 *
 * @param[in] self the pointer to the BTree
 *
 * @return the length of the BTree if it is not NULL; else 0
 */
size_t stc_btree_len(StcBTree *self);

/**
 * Insert the key and value into the BTree only if the key is not already in the
 * BTree.
 *
 * @param[in] self the pointer to the BTree
 * @param[in] key  the key to insert into the BTree
 * @param[in] val  the value to insert into the BTree
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_btree_insert(StcBTree *self, void *key, void *val);

/**
 * Get the value corresponding to the key from the BTree if the key is contained
 * in the BTree.
 *
 * @param[in] self the pointer to the BTree
 * @param[in] key  the key to get the corresponding value of from the BTree
 *
 * @return the value corresponding to the key if found; else NULL
 */
void *stc_btree_get(StcBTree *self, void *key);

/**
 * Check whether the key is contained in the BTree.
 *
 * @param[in] self the pointer to the BTree
 * @param[in] key  the key to check for in the BTree
 *
 * @return non-zero value if the key was found; else 0 if it was not found
 */
int stc_btree_contains_key(StcBTree *self, void *key);

/**
 * Remove the key-value pair from the BTree if the key is contained in the BTree
 * and returns the value corresponding to the key.
 *
 * @param[in] self    the pointer to the BTree
 * @param[in] key     the key of the key-value pair to remove from the BTree
 * @param[in] keyfree the function used to free the key in the BTree if found
 *
 * @return the value corresponding to the key if the key is contained in the
 *         BTree; else NULL if it is not
 */
void *stc_btree_remove(StcBTree                   *self,
                       void                       *key,
                       stc_btree_keyval_free_func *keyfree);

/**
 * Create an ordered array of the keys contained in the BTree.
 *
 * @param[in] self the pointer to the BTree
 *
 * @return an ordered array of the keys contained in the BTree with the same
 *         length as the BTree
 */
void **stc_btree_keys(StcBTree *self);

/**
 * Create an array of the values contained in the BTree ordered by their
 * corresponding keys in the BTree.
 *
 * @param[in] self the pointer to the BTree
 *
 * @return an array of the values contained in the BTree ordered by their
 *         corresponding keys in the BTree with the same length as the BTree
 */
void **stc_btree_values(StcBTree *self);

/**
 * Free the memory occupied by the BTree, freeing the memory of each key and
 * value in the BTree.
 *
 * @param[in] self    the pointer to the BTree
 * @param[in] keyfree the function used to free each key in the BTree
 * @param[in] valfree the function used to free each value in the BTree
 */
void stc_btree_free(StcBTree                   *self,
                    stc_btree_keyval_free_func *keyfree,
                    stc_btree_keyval_free_func *valfree);

#ifndef STC_BTREE_DISABLE_MAP
/**
 * Wrap the BTree inside a map "interface" but as a borrowed reference,
 * i.e., it cannot be freed from the map wrapper.
 *
 * @param[in] self the pointer to the BTree
 *
 * @return a pointer to the map "interface" that wraps around the BTree
 */
StcMap *stc_btree_as_map(StcBTree *self);

/**
 * Wrap the BTree inside a map "interface" giving full ownership to the map,
 * i.e., it can and must be freed from the map wrapper.
 *
 * @param[in] self the pointer to the BTree
 *
 * @return a pointer to the map "interface" that wraps around the BTree
 */
StcMap *stc_btree_to_map(StcBTree *self);
#endif /* STC_BTREE_DISABLE_MAP */

#endif /* STC_BTREE_H */
