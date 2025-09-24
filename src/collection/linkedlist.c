#include <stdbool.h>
#include <stdlib.h>

#include <stc/collection/linkedlist.h>

/* --- Type definitions ----------------------------------------------------- */

typedef struct stc_linkedlist_node StcLinkedListNode;

struct stc_linkedlist_node {
    void              *elem;
    StcLinkedListNode *prev;
    StcLinkedListNode *next;
};

struct stc_linkedlist {
    size_t                   len;
    StcLinkedListNode       *sentinel;
    stc_linkedlist_cmp_func *cmp;
};

/* --- Helper function prototypes ------------------------------------------- */

static StcLinkedListNode *stc_linkedlist_find(StcLinkedList *self,
                                              void          *target);
static StcLinkedListNode *stc_linkedlist_find_at(StcLinkedList *self,
                                                 size_t         idx);
static int                stc_linkedlist_add_after(StcLinkedList     *self,
                                                   void              *elem,
                                                   StcLinkedListNode *target);
static void              *stc_linkedlist_remove_node(StcLinkedList     *self,
                                                     StcLinkedListNode *target);

/* --- Function definitions ------------------------------------------------- */

StcLinkedList *stc_linkedlist_new_with_cmp(stc_linkedlist_cmp_func *cmp)
{
    StcLinkedList *linkedlist = malloc(sizeof(*linkedlist));

    linkedlist->len            = 0;
    linkedlist->sentinel       = malloc(sizeof(*linkedlist->sentinel));
    linkedlist->sentinel->elem = NULL;
    linkedlist->sentinel->prev = linkedlist->sentinel->next =
        linkedlist->sentinel;
    linkedlist->cmp = cmp;

    return linkedlist;
}

void stc_linkedlist_free(StcLinkedList            *self,
                         stc_linkedlist_free_func *elemfree)
{
    StcLinkedListNode *p, *q;

    if (!(self && elemfree)) return;

    for (p = self->sentinel->next; p != self->sentinel; p = q) {
        q = p->next;
        elemfree(p->elem);
        free(p);
    }
    free(self->sentinel);
    free(self);
}

size_t stc_linkedlist_len(StcLinkedList *self) { return self ? self->len : 0; }

bool stc_linkedlist_is_empty(StcLinkedList *self)
{
    return self->sentinel->next == self->sentinel;
}

int stc_linkedlist_push(StcLinkedList *self, void *elem)
{
    return stc_linkedlist_add_after(self, elem, self->sentinel);
}

void *stc_linkedlist_pop(StcLinkedList *self)
{
    if (stc_linkedlist_is_empty(self)) return NULL;
    return stc_linkedlist_remove_node(self, self->sentinel->next);
}

int stc_linkedlist_enqueue(StcLinkedList *self, void *elem)
{
    return stc_linkedlist_add_after(self, elem, self->sentinel->prev);
}

void *stc_linkedlist_dequeue(StcLinkedList *self)
{
    if (stc_linkedlist_is_empty(self)) return NULL;
    return stc_linkedlist_remove_node(self, self->sentinel->prev);
}

void *stc_linkedlist_first(StcLinkedList *self)
{
    return self->sentinel->next->elem;
}

void *stc_linkedlist_last(StcLinkedList *self)
{
    return self->sentinel->prev->elem;
}

int stc_linkedlist_insert_after(StcLinkedList *self, void *elem, void *target)
{
    StcLinkedListNode *node = stc_linkedlist_find(self, target);
    return node ? stc_linkedlist_add_after(self, elem, node)
                : STC_LINKEDLIST_INSERT_TARGET_NOT_FOUND;
}

int stc_linkedlist_insert_before(StcLinkedList *self, void *elem, void *target)
{
    StcLinkedListNode *node = stc_linkedlist_find(self, target);
    return node ? stc_linkedlist_add_after(self, elem, node->prev)
                : STC_LINKEDLIST_INSERT_TARGET_NOT_FOUND;
}

void stc_linkedlist_remove_elem(StcLinkedList            *self,
                                void                     *elem,
                                stc_linkedlist_free_func *elemfree)
{
    StcLinkedListNode *node;

    if (elemfree == NULL) return;

    node = stc_linkedlist_find(self, elem);
    if (node) elemfree(stc_linkedlist_remove_node(self, node));
}

int stc_linkedlist_insert(StcLinkedList *self, size_t idx, void *elem)
{
    StcLinkedListNode *node = stc_linkedlist_find_at(self, idx);
    return node ? stc_linkedlist_add_after(self, elem, node->prev)
                : STC_LINKEDLIST_INSERT_INDEX_OUT_OF_BOUNDS;
}

void *stc_linkedlist_get(StcLinkedList *self, size_t idx)
{
    StcLinkedListNode *node = stc_linkedlist_find_at(self, idx);
    return node ? node->elem : NULL;
}

void *stc_linkedlist_remove(StcLinkedList *self, size_t idx)
{
    StcLinkedListNode *node = stc_linkedlist_find_at(self, idx);
    return node ? stc_linkedlist_remove_node(self, node) : NULL;
}

bool stc_linkedlist_contains(StcLinkedList *self, void *elem)
{
    return stc_linkedlist_find(self, elem) != NULL;
}

/* --- Helper function definitions ------------------------------------------ */

static StcLinkedListNode *stc_linkedlist_find(StcLinkedList *self, void *target)
{
    StcLinkedListNode *node;

    if (!(self && self->cmp && target)) return NULL;

    for (node = self->sentinel->next; node != self->sentinel; node = node->next)
        if (self->cmp(target, node->elem) == 0) return node;

    return NULL;
}

static StcLinkedListNode *stc_linkedlist_find_at(StcLinkedList *self,
                                                 size_t         idx)
{
    size_t             i;
    StcLinkedListNode *node;

    if (idx >= self->len) return NULL;

    if (idx < self->len / 2)
        for (i = 0, node = self->sentinel->next; i < idx;
             i++, node   = node->next);
    else
        for (i = self->len - 1, node = self->sentinel->prev; i > idx;
             i--, node               = node->prev);

    return node;
}

static int stc_linkedlist_add_after(StcLinkedList     *self,
                                    void              *elem,
                                    StcLinkedListNode *target)
{
    StcLinkedListNode *node;

    if (target == NULL) return STC_LINKEDLIST_INSERT_TARGET_NOT_FOUND;

    node             = malloc(sizeof(*node));
    node->elem       = elem;
    node->prev       = target;
    node->next       = target->next;
    target->next     = node;
    node->next->prev = node;
    self->len++;

    return STC_LINKEDLIST_SUCCESS;
}

static void *stc_linkedlist_remove_node(StcLinkedList     *self,
                                        StcLinkedListNode *target)
{
    void *elem = target->elem;

    target->prev->next = target->next;
    target->next->prev = target->prev;
    self->len--;
    free(target);

    return elem;
}
