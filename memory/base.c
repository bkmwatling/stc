#include "base.h"

/* redefine these macros from common.h to avoid need of inclusion */
#define STC_MEM_ALIGN_UP_POW2(x, pow2) (((x) + (pow2) -1) & ~((pow2) -1))
#define STC_MEM_CLAMP_TOP(x, y)        ((x) < (y) ? (x) : (y))

void stc_mem_modify_noop(void *ctx, void *p, size_t size)
{
    (void) ctx;
    (void) p;
    (void) size;
}

StcMemArena stc_mem_arena_new(StcMemManager *man, size_t size)
{
    StcMemArena arena       = { 0 };
    arena.man               = man;
    arena.mem               = man->alloc(man->ctx, size);
    arena.cap               = size;
    arena.commit_block_size = STC_MEM_DEFAULT_COMMIT_BLOCK_SIZE;
    return arena;
}

void stc_mem_arena_free(StcMemArena *self)
{
    StcMemManager *man = self->man;
    man->free(man->ctx, self->mem, self->cap);
}

void *stc_mem_arena_push(StcMemArena *self, size_t size)
{
    void  *p;
    size_t next_commit_pos, commit_size;

    if (self->pos + size > self->cap) return NULL;

    p          = self->mem + self->pos;
    self->pos += size;
    if (self->pos > self->commit_pos) {
        next_commit_pos =
            STC_MEM_ALIGN_UP_POW2(self->pos, self->commit_block_size);
        next_commit_pos = STC_MEM_CLAMP_TOP(next_commit_pos, self->cap);
        commit_size     = next_commit_pos - self->commit_pos;

        self->man->commit(self->man->ctx, self->mem + self->commit_pos,
                          commit_size);
        self->commit_pos = next_commit_pos;
    }

    return p;
}

void *stc_mem_arena_push_zero(StcMemArena *self, size_t size)
{
    void *p = stc_mem_arena_push(self, size);
    stc_memzero(p, size);
    return p;
}

void stc_mem_arena_pop_to(StcMemArena *self, size_t pos)
{
    size_t next_commit_pos, decommit_size;

    if (pos >= self->pos) return;

    self->pos       = pos;
    next_commit_pos = STC_MEM_ALIGN_UP_POW2(self->pos, self->commit_block_size);
    next_commit_pos = STC_MEM_CLAMP_TOP(next_commit_pos, self->cap);
    if (next_commit_pos < self->commit_pos) {
        decommit_size = self->commit_pos - next_commit_pos;
        self->man->commit(self->man->ctx, self->mem + next_commit_pos,
                          decommit_size);
        self->commit_pos = next_commit_pos;
    }
}

void _stc_mem_arena_align(StcMemArena *self,
                          size_t       pow2_align,
                          void *(*arena_push)(StcMemArena *, size_t))
{
    size_t pos_aligned = STC_MEM_ALIGN_UP_POW2(self->pos, pow2_align);
    size_t size        = pos_aligned - self->pos;
    if (size > 0) arena_push(self, size);
}

#undef STC_MEM_ALIGN_UP_POW2
#undef STC_MEM_CLAMP_TOP
