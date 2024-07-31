#include <stdlib.h>

#include "hashmap.h"

/* --- Type definitions ----------------------------------------------------- */

typedef struct stc_hashmap_entry StcHashMapEntry;

struct stc_hashmap_entry {
    void            *key;
    void            *val;
    StcHashMapEntry *next;
};

struct stc_hashmap {
    StcHashMapEntry        **hashtable;
    size_t                   cap;
    size_t                   len;
    float                    max_loadfactor;
    unsigned short           idx;
    stc_hashmap_hash_func   *hash;
    stc_hashmap_keycmp_func *keycmp;
};

/* --- Helper function prototypes ------------------------------------------- */

static StcHashMapEntry **stc_hashmap_talloc(size_t tcap);
static void              stc_hashmap_rehash(StcHashMap *self);

/**
 * The array of differences between a power-of-two and the largest prime less
 * than that power-of-two.
 */
static unsigned short stc_hashmap_delta[] = {
    0,   0,  1,   1,  3,  1, 3,  1,  5,  3,  3,  9,  3,  1,   3,  19, 15,
    1,   5,  1,   3,  9,  3, 15, 3,  39, 5,  39, 57, 3,  35,  1,  5,  9,
    41,  31, 5,   25, 45, 7, 87, 21, 11, 57, 17, 55, 21, 115, 59, 81, 27,
    129, 47, 111, 33, 55, 5, 13, 27, 55, 93, 1,  57, 25, 59
};

#define STC_HASHMAP_INIT_DELTA_IDX 4
#define STC_HASHMAP_MAX_DELTA_IDX  (sizeof(stc_hashmap_delta) / sizeof(short))
#define STC_HASHMAP_GETCAP(idx)    ((1 << (idx)) - stc_hashmap_delta[idx])

#define STC_HASHMAP_LOADFACTOR(hm) ((float) (hm)->len / (float) (hm)->cap)

/* --- Function definitions ------------------------------------------------- */

StcHashMap *stc_hashmap_new_with_capacity(size_t                   cap,
                                          float                    loadfactor,
                                          stc_hashmap_hash_func   *hash,
                                          stc_hashmap_keycmp_func *keycmp)
{
    StcHashMap *hashmap;
    size_t      i;

    if (!(hash && keycmp)) return NULL;

    if (loadfactor <= 0.0f || (hashmap = malloc(sizeof(StcHashMap))) == NULL)
        return NULL;

    if (cap == STC_HASHMAP_DEFAULT_CAPACITY) {
        hashmap->idx = STC_HASHMAP_INIT_DELTA_IDX;
        hashmap->cap = cap;
    } else {
        for (i = 0; i < STC_HASHMAP_MAX_DELTA_IDX &&
                    (hashmap->cap = STC_HASHMAP_GETCAP(i)) < cap;
             i++)
            ;
        hashmap->idx = i;
    }

    if ((hashmap->hashtable = stc_hashmap_talloc(hashmap->cap)) == NULL) {
        free(hashmap);
        return NULL;
    }
    for (i = 0; i < hashmap->cap; i++) hashmap->hashtable[i] = NULL;

    hashmap->len            = 0;
    hashmap->max_loadfactor = loadfactor;
    hashmap->hash           = hash;
    hashmap->keycmp         = keycmp;

    return hashmap;
}

size_t stc_hashmap_len(StcHashMap *self) { return self ? self->len : 0; }

int stc_hashmap_insert(StcHashMap *self, void *key, void *val)
{
    size_t           hash;
    StcHashMapEntry *p;

    if (!(self && key && val)) return STC_HASHMAP_NULL_ARGUMENT;

    hash = self->hash(key, self->cap);
    for (p = self->hashtable[hash]; p; p = p->next) {
        if (self->keycmp(key, p->key) == 0)
            return STC_HASHMAP_KEY_VALUE_PAIR_EXISTS;
    }

    /* rehash if possible and necessary */
    if (STC_HASHMAP_LOADFACTOR(self) > self->max_loadfactor &&
        self->idx < STC_HASHMAP_MAX_DELTA_IDX - 1) {
        self->idx++;
        stc_hashmap_rehash(self);
    }

    if ((p = malloc(sizeof(StcHashMapEntry))) == NULL)
        return STC_HASHMAP_OUT_OF_MEMORY;
    p->key                = key;
    p->val                = val;
    p->next               = self->hashtable[hash];
    self->hashtable[hash] = p;
    self->len++;

    return STC_HASHMAP_SUCCESS;
}

void *stc_hashmap_get(StcHashMap *self, void *key)
{
    size_t           hash;
    StcHashMapEntry *p;

    if (!(self && key)) return NULL;

    hash = self->hash(key, self->cap);
    for (p = self->hashtable[hash]; p; p = p->next) {
        if (self->keycmp(key, p->key) == 0) return p->val;
    }

    return NULL;
}

int stc_hashmap_contains_key(StcHashMap *self, void *key)
{
    return stc_hashmap_get(self, key) != NULL;
}

void *stc_hashmap_remove(StcHashMap                   *self,
                         void                         *key,
                         stc_hashmap_keyval_free_func *keyfree)
{
    size_t           hash;
    void            *val;
    StcHashMapEntry *p, *q = NULL;

    if (!(self && key && keyfree)) return NULL;

    hash = self->hash(key, self->cap);
    for (p = self->hashtable[hash]; p; q = p, p = p->next) {
        if (self->keycmp(key, p->key) == 0) break;
    }

    if (p == NULL) return NULL;
    self->len--;
    if (q) {
        q->next = p->next;
    } else {
        self->hashtable[hash] = p->next;
    }

    /* rehash if possible to save memory */
    if (STC_HASHMAP_LOADFACTOR(self) <= self->max_loadfactor * 0.25f &&
        self->idx > 0) {
        self->idx--;
        stc_hashmap_rehash(self);
    }

    val = p->val;
    keyfree(p->key);
    free(p);
    return val;
}

void **stc_hashmap_keys(StcHashMap *self)
{
    size_t           i, idx = 0;
    StcHashMapEntry *p;
    void           **keys;

    if (self == NULL || (keys = malloc(self->len * sizeof(void *))) == NULL)
        return NULL;
    for (i = 0; i < self->cap; i++) {
        for (p = self->hashtable[i]; p; p = p->next) keys[idx++] = p->key;
    }

    return keys;
}

void **stc_hashmap_values(StcHashMap *self)
{
    size_t           i, idx = 0;
    StcHashMapEntry *p;
    void           **vals;

    if (self == NULL || (vals = malloc(self->len * sizeof(void *))) == NULL)
        return NULL;
    for (i = 0; i < self->cap; i++) {
        for (p = self->hashtable[i]; p; p = p->next) vals[idx++] = p->val;
    }

    return vals;
}

void stc_hashmap_free(StcHashMap                   *self,
                      stc_hashmap_keyval_free_func *keyfree,
                      stc_hashmap_keyval_free_func *valfree)
{
    size_t           i;
    StcHashMapEntry *p, *q;

    if (!(self && keyfree && valfree)) return;

    /* free the nodes in the buckets */
    for (i = 0; i < self->cap; i++) {
        for (p = self->hashtable[i]; p; p = q) {
            q = p->next;
            keyfree(p->key);
            valfree(p->val);
            free(p);
        }
    }

    /* free the table and container */
    free(self->hashtable);
    free(self);
}

/* --- Map wrapper ---------------------------------------------------------- */

#ifndef STC_HASHMAP_DISABLE_MAP
STC_MAP_CREATE_WRAPPER(stc_hashmap, StcHashMap)
#endif /* STC_HASHMAP_DISABLE_MAP */

/* --- Helper function definitions ------------------------------------------ */

static StcHashMapEntry **stc_hashmap_talloc(size_t tcap)
{
    return malloc(tcap * sizeof(StcHashMapEntry *));
}

static void stc_hashmap_rehash(StcHashMap *self)
{
    size_t           hash, cap;
    size_t           i;
    StcHashMapEntry *p, *q, **hashtable;

    /* silently refuse to hash if table allocation fails */
    cap = STC_HASHMAP_GETCAP(self->idx);
    if ((hashtable = stc_hashmap_talloc(cap)) == NULL) return;
    for (i = 0; i < cap; i++) hashtable[i] = NULL;

    for (i = 0; i < self->cap; i++) {
        for (p = self->hashtable[i]; p; p = q) {
            q               = p->next;
            hash            = self->hash(p->key, cap);
            p->next         = hashtable[hash];
            hashtable[hash] = p;
        }
    }

    self->cap = cap;
    free(self->hashtable);
    self->hashtable = hashtable;
}
