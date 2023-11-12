#include <stdlib.h>

#include "hashset.h"

/*** Type definitions *********************************************************/

typedef struct stc_hashset_entry StcHashSetEntry;

struct stc_hashset_entry {
    void            *key;
    StcHashSetEntry *next;
};

struct stc_hashset {
    StcHashSetEntry        **hashtable;
    size_t                   cap;
    size_t                   len;
    float                    max_loadfactor;
    unsigned short           idx;
    stc_hashset_hash_func   *hash;
    stc_hashset_keycmp_func *keycmp;
};

/*** Helper function prototypes ***********************************************/

static StcHashSetEntry **stc_hashset_talloc(size_t tcap);
static void              stc_hashset_rehash(StcHashSet *self);

/**
 * The array of differences between a power-of-two and the largest prime less
 * than that power-of-two.
 */
static unsigned short stc_hashset_delta[] = {
    0,   0,  1,   1,  3,  1, 3,  1,  5,  3,  3,  9,  3,  1,   3,  19, 15,
    1,   5,  1,   3,  9,  3, 15, 3,  39, 5,  39, 57, 3,  35,  1,  5,  9,
    41,  31, 5,   25, 45, 7, 87, 21, 11, 57, 17, 55, 21, 115, 59, 81, 27,
    129, 47, 111, 33, 55, 5, 13, 27, 55, 93, 1,  57, 25, 59
};

#define STC_HASHSET_INIT_DELTA_IDX 4
#define STC_HASHSET_MAX_DELTA_IDX  (sizeof(stc_hashset_delta) / sizeof(short))
#define STC_HASHSET_GETCAP(idx)    ((1 << (idx)) - stc_hashset_delta[idx])

#define STC_HASHSET_LOADFACTOR(hm) ((float) (hm)->len / (float) (hm)->cap)

/*** Function definitions *****************************************************/

StcHashSet *stc_hashset_new_with_capacity(size_t                   cap,
                                          float                    loadfactor,
                                          stc_hashset_hash_func   *hash,
                                          stc_hashset_keycmp_func *keycmp)
{
    StcHashSet *hashset;
    size_t      i;

    if (!(hash && keycmp)) return NULL;

    if (loadfactor <= 0.0f || (hashset = malloc(sizeof(StcHashSet))) == NULL)
        return NULL;

    if (cap == STC_HASHSET_DEFAULT_CAPACITY) {
        hashset->idx = STC_HASHSET_INIT_DELTA_IDX;
        hashset->cap = cap;
    } else {
        for (i = 0; i < STC_HASHSET_MAX_DELTA_IDX &&
                    (hashset->cap = STC_HASHSET_GETCAP(i)) < cap;
             i++)
            ;
        hashset->idx = i;
    }

    if ((hashset->hashtable = stc_hashset_talloc(hashset->cap)) == NULL) {
        free(hashset);
        return NULL;
    }
    for (i = 0; i < hashset->cap; i++) hashset->hashtable[i] = NULL;

    hashset->len            = 0;
    hashset->max_loadfactor = loadfactor;
    hashset->hash           = hash;
    hashset->keycmp         = keycmp;

    return hashset;
}

size_t stc_hashset_len(StcHashSet *self) { return self ? self->len : 0; }

int stc_hashset_insert(StcHashSet *self, void *key)
{
    size_t           hash;
    StcHashSetEntry *p;

    if (!(self && key)) return STC_HASHSET_NULL_ARGUMENT;

    hash = self->hash(key, self->cap);
    for (p = self->hashtable[hash]; p; p = p->next) {
        if (self->keycmp(key, p->key) == 0) return STC_HASHSET_KEY_EXISTS;
    }

    /* rehash if possible and necessary */
    if (STC_HASHSET_LOADFACTOR(self) > self->max_loadfactor &&
        self->idx < STC_HASHSET_MAX_DELTA_IDX - 1) {
        self->idx++;
        stc_hashset_rehash(self);
    }

    if ((p = malloc(sizeof(StcHashSetEntry))) == NULL)
        return STC_HASHSET_OUT_OF_MEMORY;
    p->key                = key;
    p->next               = self->hashtable[hash];
    self->hashtable[hash] = p;
    self->len++;

    return STC_HASHSET_SUCCESS;
}

int stc_hashset_contains(StcHashSet *self, void *key)
{
    size_t           hash;
    StcHashSetEntry *p;

    if (!(self && key)) return 0;

    hash = self->hash(key, self->cap);
    for (p = self->hashtable[hash]; p; p = p->next) {
        if (self->keycmp(key, p->key) == 0) return 1;
    }

    return 0;
}

void stc_hashset_remove(StcHashSet               *self,
                        void                     *key,
                        stc_hashset_keyfree_func *keyfree)
{
    size_t           hash;
    void            *val;
    StcHashSetEntry *p, *q = NULL;

    if (!(self && key && keyfree)) return;

    hash = self->hash(key, self->cap);
    for (p = self->hashtable[hash]; p; q = p, p = p->next) {
        if (self->keycmp(key, p->key) == 0) break;
    }

    if (p == NULL) return;
    self->len--;
    if (q) {
        q->next = p->next;
    } else {
        self->hashtable[hash] = p->next;
    }

    /* rehash if possible to save memory */
    if (STC_HASHSET_LOADFACTOR(self) <= self->max_loadfactor * 0.25f &&
        self->idx > 0) {
        self->idx--;
        stc_hashset_rehash(self);
    }

    keyfree(p->key);
    free(p);
}

void **stc_hashset_keys(StcHashSet *self)
{
    size_t           i, idx = 0;
    StcHashSetEntry *p;
    void           **keys;

    if (self == NULL || (keys = malloc(self->len * sizeof(void *))) == NULL)
        return NULL;
    for (i = 0; i < self->cap; i++) {
        for (p = self->hashtable[i]; p; p = p->next) keys[idx++] = p->key;
    }

    return keys;
}

void stc_hashset_free(StcHashSet *self, stc_hashset_keyfree_func *keyfree)
{
    size_t           i;
    StcHashSetEntry *p, *q;

    if (!(self && keyfree)) return;

    /* free the nodes in the buckets */
    for (i = 0; i < self->cap; i++) {
        for (p = self->hashtable[i]; p; p = q) {
            q = p->next;
            keyfree(p->key);
            free(p);
        }
    }

    /* free the table and container */
    free(self->hashtable);
    free(self);
}

/*** Set wrapper **************************************************************/

#ifndef STC_HASHSET_DISABLE_SET
STC_SET_CREATE_WRAPPER(stc_hashset, StcHashSet)
#endif /* STC_HASHSET_DISABLE_SET */

/*** Helper function definitions **********************************************/

static StcHashSetEntry **stc_hashset_talloc(size_t tcap)
{
    return malloc(tcap * sizeof(StcHashSetEntry *));
}

static void stc_hashset_rehash(StcHashSet *self)
{
    size_t           hash, cap;
    size_t           i;
    StcHashSetEntry *p, *q, **hashtable;

    /* silently refuse to hash if table allocation fails */
    cap = STC_HASHSET_GETCAP(self->idx);
    if ((hashtable = stc_hashset_talloc(cap)) == NULL) return;
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
