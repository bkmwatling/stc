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

void stc_mem_arena_free(StcMemArena *arena)
{
    StcMemManager *man = arena->man;
    man->free(man->ctx, arena->mem, arena->cap);
}

void *stc_mem_arena_push(StcMemArena *arena, size_t size)
{
    void  *p;
    size_t next_commit_pos, commit_size;

    if (arena->pos + size > arena->cap) return NULL;

    p           = arena->mem + arena->pos;
    arena->pos += size;
    if (arena->pos > arena->commit_pos) {
        next_commit_pos =
            STC_MEM_ALIGN_UP_POW2(arena->pos, arena->commit_block_size);
        next_commit_pos = STC_MEM_CLAMP_TOP(next_commit_pos, arena->cap);
        commit_size     = next_commit_pos - arena->commit_pos;

        arena->man->commit(arena->man->ctx, arena->mem + arena->commit_pos,
                           commit_size);
        arena->commit_pos = next_commit_pos;
    }

    return p;
}

void *stc_mem_arena_push_zero(StcMemArena *arena, size_t size)
{
    void *p = stc_mem_arena_push(arena, size);
    stc_memzero(p, size);
    return p;
}

void stc_mem_arena_pop_to(StcMemArena *arena, size_t pos)
{
    size_t next_commit_pos, decommit_size;

    if (pos >= arena->pos) return;

    arena->pos = pos;
    next_commit_pos =
        STC_MEM_ALIGN_UP_POW2(arena->pos, arena->commit_block_size);
    next_commit_pos = STC_MEM_CLAMP_TOP(next_commit_pos, arena->cap);
    if (next_commit_pos < arena->commit_pos) {
        decommit_size = arena->commit_pos - next_commit_pos;
        arena->man->commit(arena->man->ctx, arena->mem + next_commit_pos,
                           decommit_size);
        arena->commit_pos = next_commit_pos;
    }
}

void _stc_mem_arena_align(StcMemArena *arena,
                          size_t       pow2_align,
                          void *(*arena_push)(StcMemArena *, size_t))
{
    size_t pos_aligned = STC_MEM_ALIGN_UP_POW2(arena->pos, pow2_align);
    size_t size        = pos_aligned - arena->pos;
    if (size > 0) arena_push(arena, size);
}

#undef STC_MEM_ALIGN_UP_POW2
#undef STC_MEM_CLAMP_TOP
