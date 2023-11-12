#ifndef STC_LINKEDLIST_H
#define STC_LINKEDLIST_H

#include <stddef.h>

typedef struct stc_linkedlist StcLinkedList;

typedef int  stc_linkedlist_cmp_func(void *item1, void *item2);
typedef void stc_linkedlist_free_func(void *);

#if defined(STC_ENABLE_SHORT_NAMES) || \
    defined(STC_LINKEDLIST_ENABLE_SHORT_NAMES)
typedef StcLinkedList LinkedList;

typedef stc_linkedlist_cmp_func  linkedlist_cmp_func;
typedef stc_linkedlist_free_func linkedlist_free_func;

#    define LINKEDLIST_INSERT_INDEX_OUT_OF_BOUNDS \
        STC_LINKEDLIST_INSERT_INDEX_OUT_OF_BOUNDS
#    define LINKEDLIST_INSERT_TARGET_NOT_FOUND \
        STC_LINKEDLIST_INSERT_TARGET_NOT_FOUND
#    define LINKEDLIST_SUCCESS STC_LINKEDLIST_SUCCESS

#    define linkedlist_new      stc_linkedlist_new
#    define linkedlist_len      stc_linkedlist_len
#    define linkedlist_is_empty stc_linkedlist_is_empty

#    define linkedlist_push stc_linkedlist_push
#    define linkedlist_pop  stc_linkedlist_pop

#    define linkedlist_enqueue stc_linkedlist_enqueue
#    define linkedlist_dequeue stc_linkedlist_dequeue

#    define linkedlist_first stc_linkedlist_first
#    define linkedlist_last  stc_linkedlist_last

#    define linkedlist_insert_before stc_linkedlist_insert_before
#    define linkedlist_insert_after  stc_linkedlist_insert_after
#    define linkedlist_remove_item   stc_linkedlist_remove_item

#    define linkedlist_insert stc_linkedlist_insert
#    define linkedlist_get    stc_linkedlist_get
#    define linkedlist_remove stc_linkedlist_remove

#    define linkedlist_contains stc_linkedlist_contains
#    define linkedlist_free     stc_linkedlist_free
#endif /* STC_LINKEDLIST_ENABLE_SHORT_NAMES */

#define STC_LINKEDLIST_INSERT_INDEX_OUT_OF_BOUNDS -2
#define STC_LINKEDLIST_INSERT_TARGET_NOT_FOUND    -1
#define STC_LINKEDLIST_SUCCESS                    0

#define stc_linkedlist_new() stc_linkedlist_new_with_cmp(NULL)

/**
 * Creates a linked list with the comparison function cmp.
 *
 * @return a pointer to the created linked list
 */
StcLinkedList *stc_linkedlist_new_with_cmp(stc_linkedlist_cmp_func *cmp);

/**
 * Gets the length of (number of items in) the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the length of the linked list
 */
size_t stc_linkedlist_len(StcLinkedList *self);

/**
 * Checks whether the linked list is empty.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return non-zero value if the linked list is empty; else 0 if it is empty
 */
int stc_linkedlist_is_empty(StcLinkedList *self);

/**
 * Pushes the item to the front of the linked list.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] item the item to push to the front of the linked list
 *
 * @return 0 if the push was successful; else non-zero value
 */
int stc_linkedlist_push(StcLinkedList *self, void *item);

/**
 * Pops (removes) the first item off from the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the item popped off from the front of the linked list
 */
void *stc_linkedlist_pop(StcLinkedList *self);

/**
 * Enqueues (adds) the item to the end of the linked list.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] item the item to enqueue to the end of the linked list
 *
 * @return 0 if the enqueue was successful; else non-zero value
 */
int stc_linkedlist_enqueue(StcLinkedList *self, void *item);

/**
 * Dequeues (removes) the last item from the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the item dequeued from the end of the linked list
 */
void *stc_linkedlist_dequeue(StcLinkedList *self);

/**
 * Gets the first item from the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the first item in the linked list
 */
void *stc_linkedlist_first(StcLinkedList *self);

/**
 * Gets the last item from the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the last item in the linked list
 */
void *stc_linkedlist_last(StcLinkedList *self);

/**
 * Inserts the item after the first occurance of target in the linked list.
 * Note: the linked list must have a non-null comparison function to use this
 * function.
 *
 * Note: the linked list must have a non-null cmp function to use this function.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] item the item to insert into the linked list
 * @param[in] target the target item to insert the item after in the linked list
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_linkedlist_insert_after(StcLinkedList *self, void *item, void *target);

/**
 * Inserts the item before the first occurance of target in the linked list.
 * Note: the linked list must have a non-null comparison function to use this
 * function.
 *
 * Note: the linked list must have a non-null cmp function to use this function.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] item the item to insert into the linked list
 * @param[in] target the target item to insert the item before in the linked
 *                   list
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_linkedlist_insert_before(StcLinkedList *self, void *item, void *target);

/**
 * Removes the first occurance of the item from the linked list if it is
 * contained in the linked list.
 *
 * Note: the linked list must have a non-null cmp function to use this function.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] item the item to remove from the linked list
 * @param[in] itemfree the function used to free the item in the linked list if
 *                     found
 */
void stc_linkedlist_remove_item(StcLinkedList            *self,
                                void                     *item,
                                stc_linkedlist_free_func *itemfree);

/**
 * Inserts the item into the linked list at the specified index.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] idx the index to insert the item at in the linked list
 * @param[in] item the item to insert into the linked list
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_linkedlist_insert(StcLinkedList *self, size_t idx, void *item);

/**
 * Gets the value at the index from the linked list if the index is less than
 * the length of the linked list.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] idx the index of item to retrieve from the linked list
 *
 * @return the item at the index if the index is less than the length of the
 *         linked list; else NULL
 */
void *stc_linkedlist_get(StcLinkedList *self, size_t idx);

/**
 * Removes the item at the index from the linked list if the index is less than
 * the length of the linked list and returns the item at the index.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] idx the index of item to remove from the linked list
 *
 * @return the item at the index if the index is less than the length of the
 *         linked list; else NULL
 */
void *stc_linkedlist_remove(StcLinkedList *self, size_t idx);

/**
 * Checks whether the item is contained in the linked list.
 *
 * Note: the linked list must have a non-null cmp function to use this function.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] item the item to check for in the linked list
 *
 * @return non-zero if the item was found; else 0 if it was not found
 */
int stc_linkedlist_contains(StcLinkedList *self, void *item);

/**
 * Frees the memory occupied by the linked list, freeing the memory of each item
 * in the linked list.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] itemfree the function used to free each item in the linked list
 */
void stc_linkedlist_free(StcLinkedList            *self,
                         stc_linkedlist_free_func *itemfree);

#endif /* STC_LINKEDLIST_H */
