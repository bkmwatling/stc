#ifndef STC_MEMORY_BASE_H
#define STC_MEMORY_BASE_H

#include <string.h>

#ifdef STC_MEM_ENABLE_SHORT_NAMES
#    define KB STC_KB
#    define MB STC_MB
#    define GB STC_GB

#    define memzero        stc_memzero
#    define MEMZERO_STRUCT STC_MEMZERO_STRUCT
#    define MEMZERO_ARRAY  STC_MEMZERO_ARRAY
#    define MEMZERO_TYPED  STC_MEMZERO_TYPED

#    define memeq         stc_memeq
#    define MEMCPY_STRUCT STC_MEMCPY_STRUCT
#    define MEMCPY_ARRAY  STC_MEMCPY_ARRAY
#    define MEMCPY_TYPED  STC_MEMCPY_TYPED

typedef StcMemManager MemManager;
#    define mem_alloc_func  stc_mem_alloc_func
#    define mem_modify_func stc_mem_modify_func
#    define mem_modify_noop stc_mem_modify_noop

typedef StcMemArena MemArena;
#    define MEM_DEFAULT_ALLOC_SIZE        STC_MEM_DEFAULT_ALLOC_SIZE
#    define MEM_DEFAULT_COMMIT_BLOCK_SIZE STC_MEM_DEFAULT_COMMIT_BLOCK_SIZE

#    define mem_arena_default         stc_mem_arena_default
#    define mem_arena_push_array      stc_mem_arena_push_array
#    define mem_arena_push_array_zero stc_mem_arena_push_array_zero
#    define mem_arena_pop             stc_mem_arena_pop
#    define mem_arena_align           stc_mem_arena_align
#    define mem_arena_align_zero      stc_mem_arena_align_zero

#    define mem_arena_new       stc_mem_arena_new
#    define mem_arena_free      stc_mem_arena_free
#    define mem_arena_push      stc_mem_arena_push
#    define mem_arena_push_zero stc_mem_arena_push_zero
#    define mem_arena_pop_to    stc_mem_arena_pop_to
#endif /* STC_MEM_ENABLE_SHORT_NAMES */

/*** Simple useful macros *****************************************************/

#define STC_KB(n) ((n) << 10)
#define STC_MB(n) ((n) << 20)
#define STC_GB(n) ((n) << 30)

#define stc_memzero(p, size)      memset((p), 0, (size))
#define STC_MEMZERO_STRUCT(p)     stc_memzero((p), sizeof(*(p)))
#define STC_MEMZERO_ARRAY(p)      stc_memzero((p), sizeof(p))
#define STC_MEMZERO_TYPED(p, len) stc_memzero((p), sizeof(*(p)) * (len))

#define stc_memeq(x, y, size) (memcmp((x), (y), (size)) == 0)
#define STC_MEMCPY_STRUCT(p, q) \
    memcpy((p), (q), STC_MIN(sizeof(*(p)), sizeof(*(q))))
#define STC_MEMCPY_ARRAY(p, q) memcpy((p), (q), STC_MIN(sizeof(p), sizeof(q)))
#define STC_MEMCPY_TYPED(p, q, size) \
    memcpy((p), (q), STC_MIN(sizeof(*(p)), sizeof(*(q))) * (size))

/*** Memory V-Table ***********************************************************/

/*** Types ***/

typedef void *stc_mem_alloc_func(void *ctx, size_t size);
typedef void  stc_mem_modify_func(void *ctx, void *p, size_t size);

typedef struct {
    stc_mem_alloc_func  *alloc;
    stc_mem_modify_func *commit;
    stc_mem_modify_func *decommit;
    stc_mem_modify_func *free;
    void                *ctx;
} StcMemManager;

/*** Helper functions ***/

/**
 * Performs no operation, which is all that is necessary for some memory
 * management implementations, e.g. LibC.
 *
 * @param[in] ctx the context for the memory
 * @param[in] p the pointer to the memory to modify
 * @param[in] size the size of the memory pointed to by p
 */
void stc_mem_modify_noop(void *ctx, void *p, size_t size);

/*** Memory Arena *************************************************************/

/*** Types ***/

typedef struct {
    StcMemManager *man;
    char          *mem;
    size_t         cap;
    size_t         pos;
    size_t         commit_pos;
    size_t         commit_block_size;
} StcMemArena;

/*** Macros and functions ***/

#define STC_MEM_DEFAULT_ALLOC_SIZE        STC_GB(1)
#define STC_MEM_DEFAULT_COMMIT_BLOCK_SIZE STC_MB(64)

#define stc_mem_arena_default(man) \
    stc_mem_arena_new(man, STC_MEM_DEFAULT_ALLOC_SIZE)
#define stc_mem_arena_push_array(arena, T, len) \
    stc_mem_arena_push((arena), sizeof(T) * (len))
#define stc_mem_arena_push_array_zero(arena, T, len) \
    stc_mem_arena_push_zero((arena), sizeof(T) * (len))
#define stc_mem_arena_pop(arena, size) \
    stc_mem_arena_pop_to((arena), (arena)->pos - (size))
#define stc_mem_arena_align(arena, size) \
    _stc_mem_arena_align((arena), (size), stc_mem_arena_push)
#define stc_mem_arena_align_zero(arena, size) \
    _stc_mem_arena_align((arena), (size), stc_mem_arena_push_zero)

/**
 * Creates a new memory arena from a memory manager, allocating space of size.
 *
 * @param[in] man the pointer to the memory manager
 * @param[in] size the size of the space to allocate
 *
 * @return the memory arena using the memory manager with size space allocated
 */
StcMemArena stc_mem_arena_new(StcMemManager *man, size_t size);

/**
 * Frees the memory of the memory arena using its memory manager.
 *
 * @param[in] arena the memory arena to free
 */
void stc_mem_arena_free(StcMemArena *arena);

/**
 * Allocate (push) size amount of a memory arena's memory.
 *
 * @param[in] arena the pointer to the memory arena
 * @param[in] size the size of memory to allocate
 *
 * @return a pointer to the allocated memory of the memory arena
 */
void *stc_mem_arena_push(StcMemArena *arena, size_t size);

/**
 * Allocate (push) size amount of a memory arena's memory, and zero it out.
 *
 * @param[in] arena the pointer to the memory arena
 * @param[in] size the size of memory to allocate
 *
 * @return a pointer to the zeroed out allocated memory of the memory arena
 */
void *stc_mem_arena_push_zero(StcMemArena *arena, size_t size);

/**
 * Deallocate (pop) memory to position pos in the memory arena's memory.
 *
 * @param[in] arena the pointer to the memory arena
 * @param[in] pos the position to pop to
 */
void stc_mem_arena_pop_to(StcMemArena *arena, size_t pos);

/**
 * Aligns the memory arena position to a given power of 2.
 * Note: this function should not be used directly, but rather through the use
 * of the defined macros stc_mem_arena_align and stc_mem_arena_align_zero.
 *
 * @param[in] arena the pointer to the memory arena
 * @param[in] pow2_align the power of 2 to align to
 * @param[in] arena_push the memory push function for the memory arena
 */
void _stc_mem_arena_align(StcMemArena *arena,
                          size_t       pow2_align,
                          void *(*arena_push)(StcMemArena *, size_t));

#endif /* STC_MEMORY_BASE_H */
