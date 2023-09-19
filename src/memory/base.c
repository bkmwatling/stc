#include "base.h"

void stc_mem_modify_noop(void *ctx, void *p, u64 size) {}

StcMemArena stc_mem_arena_new(StcMemManager *man, u64 size)
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

void *stc_mem_arena_push(StcMemArena *arena, u64 size)
{
    void *p;
    u64   next_commit_pos, commit_size;

    if (arena->pos + size > arena->cap) return NULL;

    p           = arena->mem + arena->pos;
    arena->pos += size;
    if (arena->pos > arena->commit_pos) {
        next_commit_pos =
            STC_ALIGN_UP_POW2(arena->pos, arena->commit_block_size);
        next_commit_pos = STC_CLAMP_TOP(next_commit_pos, arena->cap);
        commit_size     = next_commit_pos - arena->commit_pos;

        arena->man->commit(arena->man->ctx, arena->mem + arena->commit_pos,
                           commit_size);
        arena->commit_pos = next_commit_pos;
    }

    return p;
}

void *stc_mem_arena_push_zero(StcMemArena *arena, u64 size)
{
    void *p = stc_mem_arena_push(arena, size);
    stc_memzero(p, size);
    return p;
}

void stc_mem_arena_pop_to(StcMemArena *arena, u64 pos)
{
    u64 next_commit_pos, decommit_size;

    if (pos >= arena->pos) return;

    arena->pos      = pos;
    next_commit_pos = STC_ALIGN_UP_POW2(arena->pos, arena->commit_block_size);
    next_commit_pos = STC_CLAMP_TOP(next_commit_pos, arena->cap);
    if (next_commit_pos < arena->commit_pos) {
        decommit_size = arena->commit_pos - next_commit_pos;
        arena->man->commit(arena->man->ctx, arena->mem + next_commit_pos,
                           decommit_size);
        arena->commit_pos = next_commit_pos;
    }
}

void _stc_mem_arena_align(StcMemArena *arena,
                          u64          pow2_align,
                          void *(*arena_push)(StcMemArena *, u64))
{
    u64 pos_aligned = STC_ALIGN_UP_POW2(arena->pos, pow2_align);
    u64 size        = pos_aligned - arena->pos;
    if (size > 0) arena_push(arena, size);
}
