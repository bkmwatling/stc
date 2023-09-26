#ifndef STC_BTREE_H
#define STC_BTREE_H

#ifndef STC_BTREE_DISABLE_MAP
#    include "map.h"
#endif

#ifdef STC_BTREE_ENABLE_SHORT_NAMES
typedef StcBTree BTree;

#    define btree_keycmp_func      stc_btree_keycmp_func
#    define btree_keyval_free_func stc_btree_keyval_free_func

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

typedef struct stc_btree StcBTree;

typedef int  stc_btree_keycmp_func(void *key1, void *key2);
typedef void stc_btree_keyval_free_func(void *);

#define stc_btree_is_empty(btree) (stc_btree_len(btree) == 0)

/**
 * Creates a btree with key comparison function keycmp.
 *
 * @param[in] keycmp the key comparison function to use
 *
 * @return a pointer to the created btree
 */
StcBTree *stc_btree_new(stc_btree_keycmp_func *keycmp);

/**
 * Gets the length of (number of entries in) the btree.
 *
 * @param[in] btree the pointer to the btree
 *
 * @return the length of the btree
 */
size_t stc_btree_len(StcBTree *btree);

/**
 * Inserts the key and value into the btree only if the key is not already in
 * the btree.
 *
 * @param[in] btree the pointer to the btree
 * @param[in] key the key to insert into the btree
 * @param[in] val the value to insert into the btree
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_btree_insert(StcBTree *btree, void *key, void *val);

/**
 * Gets the value corresponding to the key from the btree if the key is
 * contained in the btree.
 *
 * @param[in] btree the pointer to the btree
 * @param[in] key the key to get the corresponding value of from the btree
 *
 * @return the value corresponding to the key if found; else NULL
 */
void *stc_btree_get(StcBTree *btree, void *key);

/**
 * Checks whether the key is contained in the btree.
 *
 * @param[in] btree the pointer to the btree
 * @param[in] key the key to check for in the btree
 *
 * @return non-zero if the key was found; else 0 if it was not found
 */
int stc_btree_contains_key(StcBTree *btree, void *key);

/**
 * Removes the key-value pair from the btree if the key is contained in the
 * btree and returns the value corresponding to the key.
 *
 * @param[in] btree the pointer to the btree
 * @param[in] key the key of the key-value pair to remove from the btree
 *
 * @return the value corresponding to the key if the key is contained in the
 *         btree; else NULL if it is not
 */
void *stc_btree_remove(StcBTree *btree, void *key);

/**
 * Creates an ordered array of the keys contained in the btree.
 *
 * @param[in] btree the pointer to the btree
 *
 * @return an ordered array of the keys contained in the btree with the same
 *         length as the btree
 */
void **stc_btree_keys(StcBTree *btree);

/**
 * Creates an array of the values contained in the btree ordered by their
 * corresponding keys in the btree.
 *
 * @param[in] btree the pointer to the btree
 *
 * @return an array of the values contained in the btree ordered by their
 *         corresponding keys in the btree with the same length as the btree
 */
void **stc_btree_values(StcBTree *btree);

/**
 * Frees the memory occupied by the btree, freeing the memory of each key and
 * value in the btree.
 *
 * @param[in] btree the pointer to the btree
 * @param[in] keyfree the function used to free each key in the btree
 * @param[in] valfree the function used to free each value in the btree
 */
void stc_btree_free(StcBTree                   *btree,
                    stc_btree_keyval_free_func *keyfree,
                    stc_btree_keyval_free_func *valfree);

#ifndef STC_BTREE_DISABLE_MAP
/**
 * Wraps the btree inside a map "interface" but as a borrowed reference,
 * i.e., it cannot be freed from the map wrapper.
 *
 * @param[in] btree the pointer to the btree
 *
 * @return a pointer to the map "interface" that wraps around the btree
 */
StcMap *stc_btree_as_map(StcBTree *btree);

/**
 * Wraps the btree inside a map "interface" giving full ownership to the map,
 * i.e., it can and must be freed from the map wrapper.
 *
 * @param[in] btree the pointer to the btree
 *
 * @return a pointer to the map "interface" that wraps around the btree
 */
StcMap *stc_btree_to_map(StcBTree *btree);
#endif /* STC_BTREE_DISABLE_MAP */

#endif /* STC_BTREE_H */
