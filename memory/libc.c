#include <stdlib.h>

#include "libc.h"

void *stc_mem_libc_alloc(void *ctx, size_t size)
{
    (void) ctx;
    return malloc(size);
}

void stc_mem_libc_free(void *ctx, void *p, size_t size)
{
    (void) ctx;
    (void) size;
    free(p);
}

StcMemManager *stc_mem_libc_mem_manager(void)
{
    static StcMemManager man = { 0 };

    if (man.alloc == NULL) {
        man.alloc    = stc_mem_libc_alloc;
        man.commit   = stc_mem_modify_noop;
        man.decommit = stc_mem_modify_noop;
        man.free     = stc_mem_libc_free;
    }

    return &man;
}
