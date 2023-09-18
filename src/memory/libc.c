#include <stdlib.h>

#include "libc.h"

void *stc_mem_libc_alloc(void *ctx, u64 size) { return malloc(size); }

void stc_mem_libc_free(void *ctx, void *p, u64 size) { free(p); }

StcMemManager *stc_mem_libc_mem_manager(void)
{
    STC_LOCAL StcMemManager man = { 0 };

    if (man.alloc == NULL) {
        man.alloc    = stc_mem_libc_alloc;
        man.commit   = stc_mem_modify_noop;
        man.decommit = stc_mem_modify_noop;
        man.free     = stc_mem_libc_free;
    }

    return &man;
}
