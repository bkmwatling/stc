#ifndef STC_DEQUE_H
#define STC_DEQUE_H

#include <stdlib.h>

#include <stc/util/macro.h>

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_DEQUE_ENABLE_SHORT_NAMES)
#    define Deque StcDeque

#    define deque_new  stc_deque_new
#    define deque_init stc_deque_init
#    define deque_free stc_deque_free

#    define deque_len      stc_deque_len
#    define deque_is_empty stc_deque_is_empty
#    define deque_clear    stc_deque_clear

#    define deque_push_front stc_deque_push_front
#    define deque_push_back  stc_deque_push_back
#    define deque_push       stc_deque_push
#    define deque_enqueue    stc_deque_enqueue
#    define deque_pop_front  stc_deque_pop_front
#    define deque_pop_back   stc_deque_pop_back
#    define deque_pop        stc_deque_pop
#    define deque_dequeue    stc_deque_dequeue
#endif /* STC_DEQUE_ENABLE_SHORT_NAMES */

struct stc_deque_node_header {
    struct stc_deque_node_header *prev;
    struct stc_deque_node_header *next;
};

// TODO: add allocator field once allocator type is defined
struct stc_deque_header {
    size_t                       len;
    struct stc_deque_node_header sentinel;
};

#define _stc_deque_header(q) ((struct stc_deque_header *) (q) - 1)

#define StcDeque(T) T *

// TODO: use allocator instead of malloc and free
/**
 * Create a new double ended queue with given type, allocating the necessary
 * underlying memory and setting the length to 0.
 *
 * @param[in] T the type of the deque elements
 *
 * @return the created double ended queue
 */
#define stc_deque_new(T)                                                    \
    __extension__({                                                         \
        struct stc_deque_header *_STC_MACRO_VAR(_stc_deque_new_h_) =        \
            malloc(sizeof(*_STC_MACRO_VAR(_stc_deque_new_h_)) + sizeof(T)); \
        _STC_MACRO_VAR(_stc_deque_new_h_)->len = 0;                         \
        _STC_MACRO_VAR(_stc_deque_new_h_)->sentinel.prev =                  \
            _STC_MACRO_VAR(_stc_deque_new_h_)->sentinel.next =              \
                &_STC_MACRO_VAR(_stc_deque_new_h_)->sentinel;               \
        (StcDeque(T))(_STC_MACRO_VAR(_stc_deque_new_h_) + 1);               \
    })

/**
 * Initialise a deque's memory, setting the length to 0.
 *
 * @param[in,out] qp a pointer to the deque to initialise
 */
#define stc_deque_init(qp)                                                    \
    __extension__({                                                           \
        struct stc_deque_header *_STC_MACRO_VAR(_stc_deque_init_h_) = malloc( \
            sizeof(*_STC_MACRO_VAR(_stc_deque_init_h_)) + sizeof(**(qp)));    \
        _STC_MACRO_VAR(_stc_deque_init_h_)->len = 0;                          \
        _STC_MACRO_VAR(_stc_deque_init_h_)->sentinel.prev =                   \
            _STC_MACRO_VAR(_stc_deque_init_h_)->sentinel.next =               \
                &_STC_MACRO_VAR(_stc_deque_init_h_)->sentinel;                \
        *(qp) = (StcDeque(__typeof__(**(qp))))(                               \
            _STC_MACRO_VAR(_stc_deque_init_h_) + 1);                          \
    })

/**
 * Free the memory of a deque.
 *
 * @param[in] q         the deque to free the memory of
 * @param[in] elem_free a function to free the memory of each element in the
 *                      deque
 */
#define stc_deque_free(q, elem_free)                                    \
    __extension__({                                                     \
        __auto_type _STC_MACRO_VAR(_stc_deque_free_q_) = (q);           \
        stc_deque_clear(_STC_MACRO_VAR(_stc_deque_free_q_), elem_free); \
        free(_stc_deque_header(_STC_MACRO_VAR(_stc_deque_free_q_)));    \
    })

/**
 * Get the length of a deque.
 *
 * @param[in] q the deque to get the length of
 *
 * @return the length of the deque
 */
#define stc_deque_len(q) _stc_deque_header(q)->len

/**
 * Check whether a deque is empty (has a length of 0).
 *
 * @param[in] q the deque to check if empty
 *
 * @return true if the deque is empty; else false
 */
#define stc_deque_is_empty(q) (stc_deque_len(q) == 0)

/**
 * Clear a deque to have a length of 0.
 *
 * @param[in,out] q         the deque to clear
 * @param[in]     elem_free a function to free the memory of each element in the
 *                          deque
 */
#define stc_deque_clear(q, elem_free)                                          \
    __extension__({                                                            \
        stc_deque_node_header *_STC_MACRO_VAR(_stc_deque_free_n_),             \
            *_STC_MACRO_VAR(_stc_deque_free_next_);                            \
        __auto_type _STC_MACRO_VAR(_stc_deque_free_h_) = _stc_deque_header(q); \
        for (_STC_MACRO_VAR(_stc_deque_free_n_) =                              \
                 _STC_MACRO_VAR(_stc_deque_free_h_).sentinel->next;            \
             _STC_MACRO_VAR(_stc_deque_free_n_) !=                             \
             &_STC_MACRO_VAR(_stc_deque_free_h_).sentinel;                     \
             _STC_MACRO_VAR(_stc_deque_free_n_) =                              \
                 _STC_MACRO_VAR(_stc_deque_free_next_)) {                      \
            if ((elem_free))                                                   \
                (elem_free)(*(                                                 \
                    __typeof__(q)) (_STC_MACRO_VAR(_stc_deque_free_n_) + 1));  \
            _STC_MACRO_VAR(_stc_deque_free_n_)->prev->next =                   \
                _STC_MACRO_VAR(_stc_deque_free_next_) =                        \
                    _STC_MACRO_VAR(_stc_deque_free_n_)->next;                  \
            _STC_MACRO_VAR(_stc_deque_free_n_)->next->prev =                   \
                _STC_MACRO_VAR(_stc_deque_free_n_)->prev;                      \
            free(_STC_MACRO_VAR(_stc_deque_free_n_));                          \
        }                                                                      \
        _STC_MACRO_VAR(_stc_deque_free_h_)->len = 0;                           \
    })

/**
 * Add an element to the head of a deque.
 *
 * @param[in,out] q the deque to add the element to head of
 * @param[in]     x the element to add to the deque
 */
#define stc_deque_push_front(q, x)                                             \
    __extension__({                                                            \
        __auto_type *_STC_MACRO_VAR(_stc_deque_push_front_h_) =                \
            _stc_deque_header(q);                                              \
        struct stc_deque_node_header *_STC_MACRO_VAR(                          \
            _stc_deque_push_front_n_) =                                        \
            malloc(sizeof(*_STC_MACRO_VAR(_stc_deque_push_front_n_)) +         \
                   sizeof(x));                                                 \
        _STC_MACRO_VAR(_stc_deque_push_front_n_)->prev =                       \
            &_STC_MACRO_VAR(_stc_deque_push_front_h_)->sentinel;               \
        _STC_MACRO_VAR(_stc_deque_push_front_n_)->next =                       \
            _STC_MACRO_VAR(_stc_deque_push_front_h_)->sentinel.next;           \
        _STC_MACRO_VAR(_stc_deque_push_front_h_)->sentinel.next->prev =        \
            _STC_MACRO_VAR(_stc_deque_push_front_n_);                          \
        _STC_MACRO_VAR(_stc_deque_push_front_h_)->sentinel.next =              \
            _STC_MACRO_VAR(_stc_deque_push_front_n_);                          \
        *(__typeof__(q)) (_STC_MACRO_VAR(_stc_deque_push_front_n_) + 1) = (x); \
        _STC_MACRO_VAR(_stc_deque_push_front_h_)->len++;                       \
    })

/**
 * Add an element to the tail of a deque.
 *
 * @param[in,out] q the deque to add the element to tail of
 * @param[in]     x the element to add to the deque
 */
#define stc_deque_push_back(q, x)                                             \
    __extension__({                                                           \
        __auto_type *_STC_MACRO_VAR(_stc_deque_push_back_h_) =                \
            _stc_deque_header(q);                                             \
        struct stc_deque_node_header *_STC_MACRO_VAR(                         \
            _stc_deque_push_back_n_) =                                        \
            malloc(sizeof(*_STC_MACRO_VAR(_stc_deque_push_back_n_)) +         \
                   sizeof(x));                                                \
        _STC_MACRO_VAR(_stc_deque_push_back_n_)->prev =                       \
            _STC_MACRO_VAR(_stc_deque_push_back_h_)->sentinel.prev;           \
        _STC_MACRO_VAR(_stc_deque_push_back_n_)->next =                       \
            &_STC_MACRO_VAR(_stc_deque_push_back_h_)->sentinel;               \
        _STC_MACRO_VAR(_stc_deque_push_back_h_)->sentinel.prev->next =        \
            _STC_MACRO_VAR(_stc_deque_push_back_n_);                          \
        _STC_MACRO_VAR(_stc_deque_push_back_h_)->sentinel.prev =              \
            _STC_MACRO_VAR(_stc_deque_push_back_n_);                          \
        *(__typeof__(q)) (_STC_MACRO_VAR(_stc_deque_push_back_n_) + 1) = (x); \
        _STC_MACRO_VAR(_stc_deque_push_back_h_)->len++;                       \
    })

/**
 * Push an element onto the deque as if it was a FIFO stack.
 *
 * @param[in,out] q the deque to push the element onto
 * @param[in]     x the element to push onto the deque
 */
#define stc_deque_push stc_deque_push_front

/**
 * Enqueue an element onto the deque as if it was a LIFO queue.
 *
 * @param[in,out] q the deque to enqueue the element onto
 * @param[in]     x the element to enqueue onto the deque
 */
#define stc_deque_enqueue stc_deque_push_back

/**
 * Remove and return the element at the head of a deque.
 *
 * @param[in,out] q the deque to pop the head element from
 *
 * @return the head element of the deque
 */
#define stc_deque_pop_front(q)                                              \
    __extension__({                                                         \
        __auto_type _STC_MACRO_VAR(_stc_deque_pop_front_h_) =               \
            _stc_deque_header(q);                                           \
        __auto_type _STC_MACRO_VAR(_stc_deque_pop_front_n_) =               \
            _STC_MACRO_VAR(_stc_deque_pop_front_h_)->sentinel.next;         \
        _STC_MACRO_VAR(_stc_deque_pop_front_n_)->next->prev =               \
            &_STC_MACRO_VAR(_stc_deque_pop_front_h_)->sentinel;             \
        _STC_MACRO_VAR(_stc_deque_pop_front_h_)->sentinel.next =            \
            _STC_MACRO_VAR(_stc_deque_pop_front_n_)->next;                  \
        _STC_MACRO_VAR(_stc_deque_pop_front_h_)->len--;                     \
        __auto_type _STC_MACRO_VAR(_stc_deque_pop_front_x_) =               \
            *(__typeof__(q)) (_STC_MACRO_VAR(_stc_deque_pop_front_n_) + 1); \
        free(_STC_MACRO_VAR(_stc_deque_pop_front_n_));                      \
        _STC_MACRO_VAR(_stc_deque_pop_front_x_);                            \
    })

/**
 * Remove and return the element at the tail of a deque.
 *
 * @param[in,out] q the deque to pop the tail element from
 *
 * @return the tail element of the deque
 */
#define stc_deque_pop_back(q)                                              \
    __extension__({                                                        \
        __auto_type _STC_MACRO_VAR(_stc_deque_pop_back_h_) =               \
            _stc_deque_header(q);                                          \
        __auto_type _STC_MACRO_VAR(_stc_deque_pop_back_n_) =               \
            _STC_MACRO_VAR(_stc_deque_pop_back_h_)->sentinel.prev;         \
        _STC_MACRO_VAR(_stc_deque_pop_back_n_)->prev->next =               \
            &_STC_MACRO_VAR(_stc_deque_pop_back_h_)->sentinel;             \
        _STC_MACRO_VAR(_stc_deque_pop_back_h_)->sentinel.prev =            \
            _STC_MACRO_VAR(_stc_deque_pop_back_n_)->prev;                  \
        _STC_MACRO_VAR(_stc_deque_pop_back_h_)->len--;                     \
        __auto_type _STC_MACRO_VAR(_stc_deque_pop_back_x_) =               \
            *(__typeof__(q)) (_STC_MACRO_VAR(_stc_deque_pop_back_n_) + 1); \
        free(_STC_MACRO_VAR(_stc_deque_pop_back_n_));                      \
        _STC_MACRO_VAR(_stc_deque_pop_back_x_);                            \
    })

/**
 * Pop (remove and return) the most recently pushed element of a stack deque.
 *
 * @param[in,out] q the deque to pop an element from
 *
 * @return the most recently pushed element of the stack deque
 */
#define stc_deque_pop stc_deque_pop_front

/**
 * Dequeue (remove and return) the oldest enqueued element of a queue deque.
 *
 * @param[in,out] q the deque to dequeue an element from
 *
 * @return the oldest enqueued element of the queue deque
 */
#define stc_deque_dequeue stc_deque_pop_front

/**
 * Try to remove the element at the head of a deque into out parameter.
 *
 * @param[in,out] q   the deque to pop the head element from
 * @param[out]    out a pointer to where the removed element should be popped to
 *                    if the deque is nonempty
 *
 * @return true if the deque was nonempty; else false
 */
#define stc_deque_try_pop_front(q, out)                                \
    __extension__({                                                    \
        __auto_type _STC_MACRO_VAR(stc_deque_try_pop_front_q_) = (q);  \
        stc_deque_is_empty(_STC_MACRO_VAR(stc_deque_try_pop_front_q_)) \
            ? false                                                    \
            : (((out) = stc_deque_pop_front(                           \
                    _STC_MACRO_VAR(stc_deque_try_pop_front_q_))),      \
               true);                                                  \
    })

/**
 * Try to remove the element at the tail of a deque into out parameter.
 *
 * @param[in,out] q   the deque to pop the tail element from
 * @param[out]    out a pointer to where the removed element should be popped to
 *                    if the deque is nonempty
 *
 * @return true if the deque was nonempty; else false
 */
#define stc_deque_try_pop_back(q, out)                                \
    __extension__({                                                   \
        __auto_type _STC_MACRO_VAR(stc_deque_try_pop_back_q_) = (q);  \
        stc_deque_is_empty(_STC_MACRO_VAR(stc_deque_try_pop_back_q_)) \
            ? false                                                   \
            : (((out) = stc_deque_pop_back(                           \
                    _STC_MACRO_VAR(stc_deque_try_pop_back_q_))),      \
               true);                                                 \
    })

/**
 * Try to pop (remove & save) the most recently pushed element of a stack deque.
 *
 * @param[in,out] q   the deque to pop an element from
 * @param[out]    out a pointer to where the removed element should be popped to
 *                    if the deque is nonempty
 *
 * @return true if the stack deque was nonempty; else false
 */
#define stc_deque_try_pop stc_deque_try_pop_front

/**
 * Try to dequeue (remove & save) the oldest enqueued element of a queue deque.
 *
 * @param[in,out] q   the deque to dequeue an element from
 * @param[out]    out a pointer to where the removed element should be popped to
 *                    if the deque is nonempty
 *
 * @return true if the queue deque was nonempty; else false
 */
#define stc_deque_try_dequeue stc_deque_try_pop_front

#endif /* STC_DEQUE_H */
