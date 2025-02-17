#ifndef STC_MEM_LIBC_H
#define STC_MEM_LIBC_H

#include <stc/memory/base.h>

#if defined(STC_ENABLE_SHORT_NAMES) || defined(STC_MEM_ENABLE_SHORT_NAMES)
#    define mem_libc_alloc       stc_mem_libc_alloc
#    define mem_libc_free        stc_mem_libc_free
#    define mem_libc_mem_manager stc_mem_libc_mem_manager
#endif /* STC_MEM_ENABLE_SHORT_NAMES */

/**
 * Allocate memory of given size using LibC (malloc).
 *
 * @param[in] ctx  the context of the memory allocation (ignored)
 * @param[in] size the size of memory to allocate
 *
 * @return a pointer to the allocated memory
 */
void *stc_mem_libc_alloc(void *ctx, size_t size);

/**
 * Free memory of given size using LibC (free).
 *
 * @param[in] ctx  the context of the memory free (ignored)
 * @param[in] p    the pointer to the memory to free
 * @param[in] size the size of memory to free (ignored)
 */
void stc_mem_libc_free(void *ctx, void *p, size_t size);

/**
 * Create a pointer to a statically defined memory manager that uses LibC.
 *
 * NOTE: do not free the pointer as the memory manager is static.
 *
 * @return a pointer to the statically defined memory manager
 */
StcMemManager *stc_mem_libc_mem_manager(void);

#endif /* STC_MEM_LIBC_H */
