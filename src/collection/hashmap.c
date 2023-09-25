#include <stdlib.h>

#include "hashmap.h"

/*** Type definitions *********************************************************/

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

/*** Helper function prototypes ***********************************************/

static StcHashMapEntry **stc_hashmap_talloc(size_t tcap);
static void              stc_hashmap_rehash(StcHashMap *hashmap);

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

/*** Function definitions *****************************************************/

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

size_t stc_hashmap_len(StcHashMap *hashmap)
{
    return hashmap ? hashmap->len : 0;
}

int stc_hashmap_insert(StcHashMap *hashmap, void *key, void *val)
{
    size_t           hash;
    StcHashMapEntry *p;

    if (!(hashmap && key && val)) return 1;

    hash = hashmap->hash(key, hashmap->cap);
    for (p = hashmap->hashtable[hash]; p; p = p->next) {
        if (hashmap->keycmp(key, p->key) == 0) return -1;
    }

    /* rehash if possible and necessary */
    if (STC_HASHMAP_LOADFACTOR(hashmap) > hashmap->max_loadfactor &&
        hashmap->idx < STC_HASHMAP_MAX_DELTA_IDX - 1) {
        hashmap->idx++;
        stc_hashmap_rehash(hashmap);
    }

    if ((p = malloc(sizeof(StcHashMapEntry))) == NULL) return -2;
    p->key                   = key;
    p->val                   = val;
    p->next                  = hashmap->hashtable[hash];
    hashmap->hashtable[hash] = p;
    hashmap->len++;

    return 0;
}

void *stc_hashmap_get(StcHashMap *hashmap, void *key)
{
    size_t           hash;
    StcHashMapEntry *p;

    if (!(hashmap && key)) return NULL;

    hash = hashmap->hash(key, hashmap->cap);
    for (p = hashmap->hashtable[hash]; p; p = p->next) {
        if (hashmap->keycmp(key, p->key) == 0) return p->val;
    }

    return NULL;
}

int stc_hashmap_contains_key(StcHashMap *hashmap, void *key)
{
    return stc_hashmap_get(hashmap, key) != NULL;
}

void *stc_hashmap_remove(StcHashMap *hashmap, void *key)
{
    size_t           hash;
    void            *val;
    StcHashMapEntry *p, *q = NULL;

    if (!(hashmap && key)) return NULL;

    hash = hashmap->hash(key, hashmap->cap);
    for (p = hashmap->hashtable[hash]; p; q = p, p = p->next) {
        if (hashmap->keycmp(key, p->key) == 0) break;
    }

    if (p == NULL) return NULL;
    hashmap->len--;
    if (q) {
        q->next = p->next;
    } else {
        hashmap->hashtable[hash] = p->next;
    }

    /* rehash if possible to save memory */
    if (STC_HASHMAP_LOADFACTOR(hashmap) <= hashmap->max_loadfactor * 0.25f &&
        hashmap->idx > 0) {
        hashmap->idx--;
        stc_hashmap_rehash(hashmap);
    }

    val = p->val;
    free(p);
    return val;
}

void **stc_hashmap_keys(StcHashMap *hashmap)
{
    size_t           i, idx = 0;
    StcHashMapEntry *p;
    void           **keys;

    if (hashmap == NULL ||
        (keys = malloc(hashmap->len * sizeof(void *))) == NULL)
        return NULL;
    for (i = 0; i < hashmap->cap; i++) {
        for (p = hashmap->hashtable[i]; p; p = p->next) keys[idx++] = p->key;
    }

    return keys;
}

void **stc_hashmap_values(StcHashMap *hashmap)
{
    size_t           i, idx = 0;
    StcHashMapEntry *p;
    void           **vals;

    if (hashmap == NULL ||
        (vals = malloc(hashmap->len * sizeof(void *))) == NULL)
        return NULL;
    for (i = 0; i < hashmap->cap; i++) {
        for (p = hashmap->hashtable[i]; p; p = p->next) vals[idx++] = p->val;
    }

    return vals;
}

void stc_hashmap_free(StcHashMap                   *hashmap,
                      stc_hashmap_keyval_free_func *keyfree,
                      stc_hashmap_keyval_free_func *valfree)
{
    size_t           i;
    StcHashMapEntry *p, *q;

    if (!(hashmap && keyfree && valfree)) return;

    /* free the nodes in the buckets */
    for (i = 0; i < hashmap->cap; i++) {
        for (p = hashmap->hashtable[i]; p; p = q) {
            q = p->next;
            keyfree(p->key);
            valfree(p->val);
            free(p);
        }
    }

    /* free the table and container */
    free(hashmap->hashtable);
    free(hashmap);
}

/*** Map wrapper **************************************************************/

#ifndef STC_HASHMAP_DISABLE_MAP
STC_MAP_CREATE_WRAPPER(stc_hashmap, StcHashMap)
#endif /* STC_HASHMAP_DISABLE_MAP */

/*** Helper function definitions **********************************************/

static StcHashMapEntry **stc_hashmap_talloc(size_t tcap)
{
    return malloc(tcap * sizeof(StcHashMapEntry *));
}

static void stc_hashmap_rehash(StcHashMap *hashmap)
{
    size_t           hash, cap;
    size_t           i;
    StcHashMapEntry *p, *q, **hashtable;

    /* silently refuse to hash if table allocation fails */
    if ((hashtable = stc_hashmap_talloc(
             (cap = STC_HASHMAP_GETCAP(hashmap->idx)))) == NULL) {
        return;
    }
    for (i = 0; i < cap; i++) hashtable[i] = NULL;

    for (i = 0; i < hashmap->cap; i++) {
        for (p = hashmap->hashtable[i]; p; p = q) {
            q               = p->next;
            hash            = hashmap->hash(p->key, cap);
            p->next         = hashtable[hash];
            hashtable[hash] = p;
        }
    }

    hashmap->cap = cap;
    free(hashmap->hashtable);
    hashmap->hashtable = hashtable;
}
