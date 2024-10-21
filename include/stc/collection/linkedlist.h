#ifndef STC_LINKEDLIST_H
#define STC_LINKEDLIST_H

#include <stddef.h>

typedef struct stc_linkedlist StcLinkedList;

typedef int  stc_linkedlist_cmp_func(void *elem1, void *elem2);
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

#    define linkedlist_new          stc_linkedlist_new
#    define linkedlist_new_with_cmp stc_linkedlist_new_with_cmp
#    define linkedlist_free         stc_linkedlist_free
#    define linkedlist_len          stc_linkedlist_len
#    define linkedlist_is_empty     stc_linkedlist_is_empty

#    define linkedlist_push stc_linkedlist_push
#    define linkedlist_pop  stc_linkedlist_pop

#    define linkedlist_enqueue stc_linkedlist_enqueue
#    define linkedlist_dequeue stc_linkedlist_dequeue

#    define linkedlist_first stc_linkedlist_first
#    define linkedlist_last  stc_linkedlist_last

#    define linkedlist_insert_before stc_linkedlist_insert_before
#    define linkedlist_insert_after  stc_linkedlist_insert_after
#    define linkedlist_remove_elem   stc_linkedlist_remove_elem

#    define linkedlist_insert stc_linkedlist_insert
#    define linkedlist_get    stc_linkedlist_get
#    define linkedlist_remove stc_linkedlist_remove

#    define linkedlist_contains stc_linkedlist_contains
#endif /* STC_LINKEDLIST_ENABLE_SHORT_NAMES */

#define STC_LINKEDLIST_INSERT_INDEX_OUT_OF_BOUNDS -2
#define STC_LINKEDLIST_INSERT_TARGET_NOT_FOUND    -1
#define STC_LINKEDLIST_SUCCESS                    0

#define stc_linkedlist_new() stc_linkedlist_new_with_cmp(NULL)

/**
 * Create a linked list with the comparison function cmp.
 *
 * @param[in] cmp the comparison function to compare elements in the linked list
 *
 * @return a pointer to the created linked list
 */
StcLinkedList *stc_linkedlist_new_with_cmp(stc_linkedlist_cmp_func *cmp);

/**
 * Free the memory occupied by the linked list, freeing the memory of each
 * element in the linked list.
 *
 * @param[in] self     the pointer to the linked list
 * @param[in] elemfree the function used to free each element in the linked list
 */
void stc_linkedlist_free(StcLinkedList            *self,
                         stc_linkedlist_free_func *elemfree);

/**
 * Get the length of (number of elements in) the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the length of the linked list
 */
size_t stc_linkedlist_len(StcLinkedList *self);

/**
 * Check whether the linked list is empty.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return non-zero value if the linked list is empty; else 0 if it is empty
 */
int stc_linkedlist_is_empty(StcLinkedList *self);

/**
 * Push the element to the front of the linked list.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] elem the element to push to the front of the linked list
 *
 * @return 0 if the push was successful; else non-zero value
 */
int stc_linkedlist_push(StcLinkedList *self, void *elem);

/**
 * Pop (remove) the first element off from the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the element popped off from the front of the linked list
 */
void *stc_linkedlist_pop(StcLinkedList *self);

/**
 * Enqueue (add) the element to the end of the linked list.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] elem the element to enqueue to the end of the linked list
 *
 * @return 0 if the enqueue was successful; else non-zero value
 */
int stc_linkedlist_enqueue(StcLinkedList *self, void *elem);

/**
 * Dequeue (remove) the last element from the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the element dequeued from the end of the linked list
 */
void *stc_linkedlist_dequeue(StcLinkedList *self);

/**
 * Get the first element from the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the first element in the linked list
 */
void *stc_linkedlist_first(StcLinkedList *self);

/**
 * Gets the last element from the linked list.
 *
 * @param[in] self the pointer to the linked list
 *
 * @return the last element in the linked list
 */
void *stc_linkedlist_last(StcLinkedList *self);

/**
 * Insert the element after the first occurance of target in the linked list.
 *
 * NOTE: the linked list must have a non-null comparison function to use this
 * function.
 *
 * @param[in] self   the pointer to the linked list
 * @param[in] elem   the element to insert into the linked list
 * @param[in] target the target element to insert the element after in the
 *                   linked list
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_linkedlist_insert_after(StcLinkedList *self, void *elem, void *target);

/**
 * Insert the element before the first occurance of target in the linked list.
 *
 * NOTE: the linked list must have a non-null comparison function to use this
 * function.
 *
 * @param[in] self   the pointer to the linked list
 * @param[in] elem   the element to insert into the linked list
 * @param[in] target the target element to insert the element before in the
 *                   linked list
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_linkedlist_insert_before(StcLinkedList *self, void *elem, void *target);

/**
 * Remove the first occurance of the elem from the linked list if it is
 * contained in the linked list.
 *
 * NOTE: the linked list must have a non-null comparison function to use this
 * function.
 *
 * @param[in] self     the pointer to the linked list
 * @param[in] elem     the elem to remove from the linked list
 * @param[in] elemfree the function used to free the element in the linked list
 *                     if found
 */
void stc_linkedlist_remove_elem(StcLinkedList            *self,
                                void                     *elem,
                                stc_linkedlist_free_func *elemfree);

/**
 * Insert the element into the linked list at the specified index.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] idx  the index to insert the element at in the linked list
 * @param[in] elem the element to insert into the linked list
 *
 * @return 0 if the insert was successful; else non-zero value
 */
int stc_linkedlist_insert(StcLinkedList *self, size_t idx, void *elem);

/**
 * Get the value at the index from the linked list if the index is less than
 * the length of the linked list.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] idx  the index of the element to retrieve from the linked list
 *
 * @return the element at the index if the index is less than the length of the
 *         linked list; else NULL
 */
void *stc_linkedlist_get(StcLinkedList *self, size_t idx);

/**
 * Remove the element at the index from the linked list if the index is less
 * than the length of the linked list and returns the element at the index.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] idx  the index of element to remove from the linked list
 *
 * @return the element at the index if the index is less than the length of the
 *         linked list; else NULL
 */
void *stc_linkedlist_remove(StcLinkedList *self, size_t idx);

/**
 * Check whether the element is contained in the linked list.
 *
 * NOTE: the linked list must have a non-null comparison function to use this
 * function.
 *
 * @param[in] self the pointer to the linked list
 * @param[in] elem the element to check for in the linked list
 *
 * @return non-zero if the element was found; else 0 if it was not found
 */
int stc_linkedlist_contains(StcLinkedList *self, void *elem);

#endif /* STC_LINKEDLIST_H */
