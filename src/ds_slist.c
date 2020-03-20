#include "ds/slist.h"

#include "debug.h"

#include <stdlib.h>
#include <string.h>

int
init_ezi_slist(struct ezi_slist *sl, size_t element_size)
{
    assert(sl != NULL);

    sl->head           = NULL;
    sl->tail           = NULL;
    sl->count          = 0;
    sl->__element_size = element_size;

    return 0;
}

static inline struct ezi_slist_node *
create_node(const void *data, size_t len)
{
    struct ezi_slist_node *node;

    massert(data != NULL, "Node Data Cannot be NULL");

    if ((node = (struct ezi_slist_node *)malloc(sizeof(*node))) == NULL) {
        return NULL;
    } else if ((node->data = malloc(len)) == NULL) {
        free((void *)node);
        return NULL;
    }

    node->next = NULL;
    memcpy(node->data, data, len);

    return node;
}

struct ezi_slist_node *
ezi_slist_push(struct ezi_slist *sl, const void *data)
{
    struct ezi_slist_node *node;

    massert(sl != NULL, "The list cannot be NULL");
    massert(data != NULL, "The data cannot be NULL");
    massert(SLIST_VALID(sl), "Invalid List");

    if ((node = create_node(data, sl->__element_size)) == NULL) {
        return NULL;
    }

    if (sl->head == NULL && sl->tail == NULL) {
        sl->head = sl->tail = node;
        sl->count           = 1;
    } else {
        sl->tail->next = node;
        sl->tail       = node;
        ++sl->count;
    }

    return node;
}

struct ezi_slist_node *
ezi_slist_pop(struct ezi_slist *sl)
{
    struct ezi_slist_node *node, *ptr;

    massert(sl != NULL, "The list cannot be NULL");

    if (sl->count == 0) {
        return NULL;
    } else if (sl->tail != NULL) {
        if (sl->count > 1) {
            ptr = sl->head;
            while (ptr && ptr->next != sl->tail) {
                ptr = ptr->next;
            }

            if (ptr) {
                node      = ptr->next;
                ptr->next = NULL;
                sl->tail  = ptr;

                return node;
            }
        } else {
            ptr       = sl->tail;
            ptr->next = sl->head = sl->tail = NULL;
            sl->count                       = 0;
            return ptr;
        }
    }

    return NULL;
}

struct ezi_slist_node *
ezi_slist_shift(struct ezi_slist *sl, const void *data)
{
    struct ezi_slist_node *node;

    massert(sl != NULL, "The list cannot be NULL");
    massert(data != NULL, "The data cannot be NULL");
    massert(SLIST_VALID(sl), "Invalid List");

    if ((node = create_node(data, sl->__element_size)) == NULL) {
        return NULL;
    }

    if (sl->head == NULL && sl->tail == NULL) {
        sl->head = sl->tail = node;
        sl->count           = 1;
    } else {
        node->next = sl->head;
        sl->head   = node;
        ++sl->count;
    }

    return node;
}

struct ezi_slist_node *
ezi_slist_unshift(struct ezi_slist *sl)
{
    struct ezi_slist_node *node;

    massert(sl != NULL, "The list cannot be NULL");

    if (sl->count == 0) {
        return NULL;
    } else if (sl->head != NULL) {
        node       = sl->head;
        sl->head   = node->next;
        node->next = NULL;

        if (sl->count-- == 0) {
            sl->head = sl->tail = NULL;
        }

        return node;
    }

    return NULL;
}

inline void
free_ezi_slist_node(struct ezi_slist_node *node)
{
    massert(node != NULL, "The node cannot be NULL");
    massert(node->data != NULL, "The data cannot be NULL");

    free(node->data);
    free(node);
}

void
free_ezi_slist(struct ezi_slist *sl)
{
    struct ezi_slist_node *ptr, *tmp;

    massert(sl != NULL, "The list cannot be NULL");

    for (ptr = sl->head; ptr;) {
        tmp = ptr;
        ptr = ptr->next;
        free_ezi_slist_node(tmp);
    }

    sl->head = sl->tail = NULL;
    sl->count = sl->__element_size = 0;
}
