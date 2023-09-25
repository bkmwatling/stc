#include <stdlib.h>

#include "linkedlist.h"

/*** Type definitions *********************************************************/

typedef struct stc_linkedlist_node StcLinkedListNode;

struct stc_linkedlist_node {
    void              *item;
    StcLinkedListNode *prev;
    StcLinkedListNode *next;
};

struct stc_linkedlist {
    size_t              len;
    StcLinkedListNode  *sentinal;
    stc_linkedlist_cmp *cmp;
};

/*** Helper function prototypes ***********************************************/

static StcLinkedListNode *stc_linkedlist_find(StcLinkedList *linkedlist,
                                              void          *target);
static StcLinkedListNode *stc_linkedlist_find_at(StcLinkedList *linkedlist,
                                                 size_t         idx);
static int                stc_linkedlist_add_after(StcLinkedList     *list,
                                                   void              *item,
                                                   StcLinkedListNode *target);
static void              *stc_linkedlist_remove_node(StcLinkedList     *list,
                                                     StcLinkedListNode *target);

/*** Function definitions *****************************************************/

StcLinkedList *stc_linkedlist_new_with_cmp(stc_linkedlist_cmp *cmp)
{
    StcLinkedList *linkedlist = malloc(sizeof(StcLinkedList));

    linkedlist->len            = 0;
    linkedlist->sentinal       = malloc(sizeof(StcLinkedListNode));
    linkedlist->sentinal->item = NULL;
    linkedlist->sentinal->prev = linkedlist->sentinal->next =
        linkedlist->sentinal;
    linkedlist->cmp = cmp;

    return linkedlist;
}

size_t stc_linkedlist_len(StcLinkedList *linkedlist)
{
    return linkedlist ? linkedlist->len : 0;
}

int stc_linkedlist_is_empty(StcLinkedList *linkedlist)
{
    return linkedlist->sentinal->next == linkedlist->sentinal;
}

int stc_linkedlist_push(StcLinkedList *linkedlist, void *item)
{
    return stc_linkedlist_add_after(linkedlist, item, linkedlist->sentinal);
}

void *stc_linkedlist_pop(StcLinkedList *linkedlist)
{
    if (stc_linkedlist_is_empty(linkedlist)) return NULL;

    return stc_linkedlist_remove_node(linkedlist, linkedlist->sentinal->next);
}

int stc_linkedlist_enqueue(StcLinkedList *linkedlist, void *item)
{
    return stc_linkedlist_add_after(linkedlist, item,
                                    linkedlist->sentinal->prev);
}

void *stc_linkedlist_dequeue(StcLinkedList *linkedlist)
{
    if (stc_linkedlist_is_empty(linkedlist)) return NULL;

    return stc_linkedlist_remove_node(linkedlist, linkedlist->sentinal->prev);
}

void *stc_linkedlist_first(StcLinkedList *linkedlist)
{
    return linkedlist->sentinal->next->item;
}

void *stc_linkedlist_last(StcLinkedList *linkedlist)
{
    return linkedlist->sentinal->prev->item;
}

int stc_linkedlist_insert_after(StcLinkedList *linkedlist,
                                void          *item,
                                void          *target)
{
    StcLinkedListNode *node = stc_linkedlist_find(linkedlist, target);
    return node ? stc_linkedlist_add_after(linkedlist, item, node) : 2;
}

int stc_linkedlist_insert_before(StcLinkedList *linkedlist,
                                 void          *item,
                                 void          *target)
{
    StcLinkedListNode *node = stc_linkedlist_find(linkedlist, target);
    return node ? stc_linkedlist_add_after(linkedlist, item, node->prev) : 2;
}

void stc_linkedlist_remove_item(StcLinkedList *linkedlist, void *item)
{
    StcLinkedListNode *node = stc_linkedlist_find(linkedlist, item);
    if (node) stc_linkedlist_remove_node(linkedlist, node);
}

int stc_linkedlist_insert(StcLinkedList *linkedlist, size_t idx, void *item)
{
    StcLinkedListNode *node = stc_linkedlist_find_at(linkedlist, idx);
    return node ? stc_linkedlist_add_after(linkedlist, item, node->prev) : 2;
}

void *stc_linkedlist_get(StcLinkedList *linkedlist, size_t idx)
{
    StcLinkedListNode *node = stc_linkedlist_find_at(linkedlist, idx);
    return node ? node->item : NULL;
}

void *stc_linkedlist_remove(StcLinkedList *linkedlist, size_t idx)
{
    StcLinkedListNode *node = stc_linkedlist_find_at(linkedlist, idx);
    return node ? stc_linkedlist_remove_node(linkedlist, node) : NULL;
}

int stc_linkedlist_contains(StcLinkedList *linkedlist, void *item)
{
    return stc_linkedlist_find(linkedlist, item) != NULL;
}

void stc_linkedlist_free(StcLinkedList *linkedlist, void (*itemfree)(void *))
{
    StcLinkedListNode *p, *q;

    if (!(linkedlist && itemfree)) return;

    for (p = linkedlist->sentinal->next; p != linkedlist->sentinal; p = q) {
        q = p->next;
        itemfree(p->item);
        free(p);
    }
    free(linkedlist);
}

/*** Helper function definitions **********************************************/

static StcLinkedListNode *stc_linkedlist_find(StcLinkedList *linkedlist,
                                              void          *target)
{
    StcLinkedListNode *node;

    if (!(linkedlist && linkedlist->cmp && target)) return NULL;

    for (node = linkedlist->sentinal->next; node != linkedlist->sentinal;
         node = node->next) {
        if (linkedlist->cmp(target, node->item) == 0) return node;
    }

    return NULL;
}

static StcLinkedListNode *stc_linkedlist_find_at(StcLinkedList *linkedlist,
                                                 size_t         idx)
{
    size_t             i;
    StcLinkedListNode *node;

    if (idx >= linkedlist->len) return NULL;

    if (idx < linkedlist->len / 2) {
        for (i = 0, node = linkedlist->sentinal->next; i < idx;
             i++, node   = node->next)
            ;
    } else {
        for (i = linkedlist->len - 1, node = linkedlist->sentinal->prev;
             i > idx; i--, node            = node->prev)
            ;
    }

    return node;
}

static int stc_linkedlist_add_after(StcLinkedList     *linkedlist,
                                    void              *item,
                                    StcLinkedListNode *target)
{
    StcLinkedListNode *node;

    if (target == NULL) return 1;

    node             = malloc(sizeof(StcLinkedListNode));
    node->item       = item;
    node->prev       = target;
    node->next       = target->next;
    target->next     = node;
    node->next->prev = node;
    linkedlist->len++;

    return 0;
}

static void *stc_linkedlist_remove_node(StcLinkedList     *linkedlist,
                                        StcLinkedListNode *target)
{
    void *item = target->item;

    target->prev->next = target->next;
    target->next->prev = target->prev;
    linkedlist->len--;
    free(target);

    return item;
}
