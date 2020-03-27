#include "ds/slist.h"

#include "log/errno.h"
#include "util/memory.h"
#include "util/validation.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

static inline struct ezi_slist_node *
create_node(const void *data, size_t len);

int
init_ezi_slist(struct ezi_slist *sl, size_t element_size)
{
    CHECK_NULL_PARAMS_1(sl);

    sl->head         = NULL;
    sl->tail         = NULL;
    sl->count        = 0;
    sl->element_size = element_size;

    return 0;
}

int
ezi_slist_push_no_alloc(struct ezi_slist *sl, void *data)
{
    struct ezi_slist_node *node;

    CHECK_NULL_PARAMS_1(sl);

    if (!(node = (struct ezi_slist_node *)calloc(1, sizeof(*node)))) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return -1;
    }

    if (!sl->head && !sl->tail) {
        sl->head = sl->tail = node;
        sl->count           = 1;
    } else {
        sl->tail->next = node;
        sl->tail       = node;
        ++sl->count;
    }

    node->data = data;

    return 0;
}

int
ezi_slist_push(struct ezi_slist *sl, const void *data)
{
    void *tmp;

    CHECK_NULL_PARAMS_2(sl, data);

    if (!(tmp = ezi_memdup(data, sl->element_size))) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return -1;
    }

    return ezi_slist_push_no_alloc(sl, tmp);
}

void *
ezi_slist_pop(struct ezi_slist *sl)
{
    struct ezi_slist_node *node, *ptr;
    void *                 tmp;

    node = NULL;

    if (!sl || sl->count == 0) {
        return NULL;
    } else if (sl->tail) {
        if (sl->count > 1) {
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
            sl->count                        = 0;
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
ezi_slist_shift_no_alloc(struct ezi_slist *sl, void *data)
{
    struct ezi_slist_node *node;

    CHECK_NULL_PARAMS_1(sl);

    if ((node = (struct ezi_slist_node *)calloc(1, sizeof(*node))) == NULL) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return -1;
    }

    if (!sl->head && !sl->tail) {
        sl->head = sl->tail = node;
        sl->count           = 1;
    } else {
        node->next = sl->head;
        sl->head   = node;
        ++sl->count;
    }

    node->data = data;

    return 0;
}

int
ezi_slist_shift(struct ezi_slist *sl, const void *data)
{
    void *tmp;

    CHECK_NULL_PARAMS_2(sl, data);

    if (!(tmp = ezi_memdup(data, sl->element_size))) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return -1;
    }

    return ezi_slist_shift_no_alloc(sl, tmp);
}

void *
ezi_slist_unshift(struct ezi_slist *sl)
{
    struct ezi_slist_node *node;
    void *                 tmp;

    if (!sl || sl->count == 0) {
        return NULL;
    } else if (sl->head) {
        node       = sl->head;
        sl->head   = node->next;
        node->next = NULL;

        if (--sl->count == 0) {
            sl->head = sl->tail = NULL;
        }

        tmp = node->data;
        free(node);

        return tmp;
    }

    return NULL;
}

void *
ezi_slist_remove(struct ezi_slist * sl,
                 const void *       data,
                 const void *       ctx,
                 ezi_slist_data_cmp cmp)
{
    void *                 tmp;
    struct ezi_slist_node *node, *to_remove;

    if (!sl || !data) {
        errno = EZI_ERR_NULL_ARGUMENTS;
        return NULL;
    } else if (sl->count == 0) {
        goto not_found;
    } else if ((*cmp)(sl->head->data, data, ctx) == 0) {
        if (!(tmp = ezi_slist_unshift(sl))) {
            goto not_found;
        }
        return tmp;
    } else if ((*cmp)(sl->tail->data, data, ctx) == 0) {
        if (!(tmp = ezi_slist_pop(sl))) {
            goto not_found;
        }
        return tmp;
    } else {
        SLIST_ITERATE(sl, node)
        {
            if ((*cmp)(node->data, data, ctx) == 0) {
                to_remove  = node->next;
                node->next = node->next->next;
                --sl->count;

                tmp = to_remove->data;
                free(to_remove);

                return tmp;
            }
        }
    }

not_found:
    errno = EZI_ERR_NOT_FOUND;
    return NULL;
}

void
ezi_slist_clear(struct ezi_slist *sl)
{
    struct ezi_slist_node *ptr, *tmp;

    for (ptr = sl->head; ptr;) {
        tmp = ptr;
        ptr = ptr->next;
        free_ezi_slist_node(tmp);
    }

    sl->head = sl->tail = NULL;
    sl->count = 0;
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

    memset(sl, 0, sizeof(*sl));
}

static inline struct ezi_slist_node *
create_node(const void *data, size_t len)
{
    struct ezi_slist_node *node;

    if (!data) {
        return NULL;
    } else if ((node = (struct ezi_slist_node *)malloc(sizeof(*node))) ==
               NULL) {
        return NULL;
    } else if (!(node->data = ezi_memdup(data, len))) {
        free((void *)node);
        return NULL;
    }

    node->next = NULL;
    return node;
}
