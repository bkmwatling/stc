#ifndef STC_MEM_LIBC_H
#define STC_MEM_LIBC_H

#include "base.h"

/**
 * Allocates memory of given size using LibC (malloc).
 *
 * @param[in] ctx the context of the memory allocation (ignored)
 * @param[in] size the size of memory to allocate
 *
 * @return a pointer to the allocated memory
 */
void *stc_mem_libc_alloc(void *ctx, u64 size);

/**
 * Frees memory of given size using LibC (free).
 *
 * @param[in] ctx the context of the memory free (ignored)
 * @param[in] p the pointer to the memory to free
 * @param[in] size the size of memory to free (ignored)
 */
void stc_mem_libc_free(void *ctx, void *p, u64 size);

/**
 * Creates a pointer to a statically defined memory manager that uses LibC.
 * Note: do not free the pointer as it the memory manager is static.
 *
 * @return a pointer to the statically defined memory manager
 */
StcMemManager *stc_mem_libc_mem_manager(void);

#endif /* STC_MEM_LIBC_H */
