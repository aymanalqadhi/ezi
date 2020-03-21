#include "ds/slist.h"

#include "log/errno.h"
#include "util/memory.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int
init_ezi_slist(struct ezi_slist *sl, size_t element_size)
{
    if (sl == NULL) {
        errno = EZI_ERR_NULL_ARGUMENTS;
        return -1;
    }

    sl->head           = NULL;
    sl->tail           = NULL;
    sl->__count          = 0;
    sl->__element_size = element_size;

    return 0;
}

static inline struct ezi_slist_node *
create_node(const void *data, size_t len)
{
    struct ezi_slist_node *node;

    if (data == NULL) {
        return NULL;
    } else if ((node = (struct ezi_slist_node *)malloc(sizeof(*node))) ==
               NULL) {
        return NULL;
    } else if ((node->data = ezi_memdup(data, len)) == NULL) {
        free((void *)node);
        return NULL;
    }

    node->next = NULL;
    return node;
}

int
ezi_slist_push(struct ezi_slist *sl, const void *data)
{
    struct ezi_slist_node *node;

    if (sl == NULL || data == NULL) {
        errno = EZI_ERR_NULL_ARGUMENTS;
        return -1;
    } else if ((node = create_node(data, sl->__element_size)) == NULL) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return -1;
    }

    if (sl->head == NULL && sl->tail == NULL) {
        sl->head = sl->tail = node;
        sl->__count           = 1;
    } else {
        sl->tail->next = node;
        sl->tail       = node;
        ++sl->__count;
    }

    return 0;
}

void *
ezi_slist_pop(struct ezi_slist *sl)
{
    struct ezi_slist_node *node, *ptr;
    void *                 tmp;

    node = NULL;

    if (sl == NULL || sl->__count == 0) {
        return NULL;
    } else if (sl->tail != NULL) {
        if (sl->__count > 1) {
            ptr = sl->head;
            while (ptr && ptr->next != sl->tail) {
                ptr = ptr->next;
            }

            if (ptr) {
                node      = ptr->next;
                ptr->next = NULL;
                sl->tail  = ptr;
            }
        } else {
            node       = sl->tail;
            node->next = sl->head = sl->tail = NULL;
            sl->__count                        = 0;
        }
    }

    if (node) {
        tmp = node->data;
        free(node);
        return tmp;
    }

    return NULL;
}

int
ezi_slist_shift(struct ezi_slist *sl, const void *data)
{
    struct ezi_slist_node *node;

    if (sl == NULL || data == NULL) {
        errno = EZI_ERR_NULL_ARGUMENTS;
        return -1;
    } else if ((node = create_node(data, sl->__element_size)) == NULL) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return -1;
    }

    if (sl->head == NULL && sl->tail == NULL) {
        sl->head = sl->tail = node;
        sl->__count           = 1;
    } else {
        node->next = sl->head;
        sl->head   = node;
        ++sl->__count;
    }

    return 0;
}

void *
ezi_slist_unshift(struct ezi_slist *sl)
{
    struct ezi_slist_node *node;
    void *                 tmp;

    if (sl == NULL || sl->__count == 0) {
        return NULL;
    } else if (sl->head != NULL) {
        node       = sl->head;
        sl->head   = node->next;
        node->next = NULL;

        if (sl->__count-- == 0) {
            sl->head = sl->tail = NULL;
        }

        tmp = node->data;
        free(node);

        return tmp;
    }

    return NULL;
}

inline void
free_ezi_slist_node(struct ezi_slist_node *node)
{
    free(node->data);
    free(node);
}

void
free_ezi_slist(struct ezi_slist *sl)
{
    struct ezi_slist_node *ptr, *tmp;

    for (ptr = sl->head; ptr;) {
        tmp = ptr;
        ptr = ptr->next;
        free_ezi_slist_node(tmp);
    }

    sl->head = sl->tail = NULL;
    sl->__count = sl->__element_size = 0;
}
