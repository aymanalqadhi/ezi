#include "ds/hash_table.h"
#include "ds/slist.h"

#include "log/errno.h"
#include "util/memory.h"
#include "util/validation.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define FNV_PRIME        16777619
#define FNV_OFFSET_BASIS 2166136261
#define MOD_ADLER        65521

static struct ezi_hash_table_entry *
create_hash_table_entry(const void *key,
                        size_t      key_size,
                        const void *value,
                        size_t      value_size);

static inline void
free_ezi_hash_table_entry(struct ezi_hash_table_entry *entry);

static int
compare_key(const void *k1, const void *k2, const void *sz);

int
init_ezi_hash_table_free(struct ezi_hash_table *ht,
                         size_t                 buckets_count,
                         size_t                 key_size,
                         size_t                 value_size,
                         free_func_t            free_func)
{
    int i;

    CHECK_NULL_PARAMS_1(ht);

    buckets_count = buckets_count > 0 ? buckets_count
                                      : DEFAULT_HASH_TABLE_BUCKETS;

    if (!(ht->buckets = calloc(ht->buckets_count = buckets_count,
                               sizeof(*ht->buckets)))) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return -1;
    }

    for (i = 0; i < buckets_count; ++i) {
        if (init_ezi_slist(ht->buckets + i,
                           sizeof(struct ezi_hash_table_entry)) != 0) {
            free(ht->buckets);
            errno = EZI_ERR_MEMORY_ALLOC_FAILED;
            return -1;
        }
    }

    ht->free_func  = free_func;
    ht->count      = 0;
    ht->key_size   = key_size;
    ht->value_size = value_size;
    ht->hash_func  = DEFAULT_HASH_FUNCTION;

    return 0;
}

inline int
init_ezi_hash_table(struct ezi_hash_table *ht,
                    size_t                 buckets_count,
                    size_t                 key_size,
                    size_t                 value_size)
{
    return init_ezi_hash_table_free(
        ht, buckets_count, key_size, value_size, NULL);
}

int
ezi_hash_table_get(struct ezi_hash_table *ht, const void *key, void **value_ptr)
{
    size_t                       hash;
    struct ezi_hash_table_entry *entry;
    struct ezi_slist_node *      node;

    CHECK_NULL_PARAMS_3(ht, key, value_ptr);
    CHECK_UNINITIALIZED_DATA_1(ht->buckets);

    if (ht->count == 0) {
        *value_ptr = NULL;
        return 0;
    }

    hash = (ht->hash_func)(key, ht->key_size) % ht->buckets_count;

    SLIST_ITERATE(ht->buckets + hash, node)
    {
        entry = (struct ezi_hash_table_entry *)node->data;
        if (memcmp(key, entry->key, ht->key_size) == 0) {
            *value_ptr = entry->value;
            return 0;
        }
    }

    *value_ptr = NULL;
    return 0;
}

int
ezi_hash_table_has_key(struct ezi_hash_table *ht, const void *key)
{
    size_t                       hash;
    struct ezi_hash_table_entry *entry;
    struct ezi_slist_node *      node;

    CHECK_NULL_PARAMS_2(ht, key);
    CHECK_UNINITIALIZED_DATA_1(ht->buckets);

    hash = (*ht->hash_func)(key, ht->key_size) % ht->buckets_count;

    SLIST_ITERATE(ht->buckets + hash, node)
    {
        entry = (struct ezi_hash_table_entry *)node->data;
        if (memcmp(key, entry->key, ht->key_size) == 0) {
            return 1;
        }
    }

    return 0;
}

int
ezi_hash_table_set(struct ezi_hash_table *ht,
                   const void *           key,
                   const void *           value)
{
    size_t                       hash;
    struct ezi_hash_table_entry *entry;
    struct ezi_slist_node *      node;

    CHECK_NULL_PARAMS_3(ht, key, value);
    CHECK_UNINITIALIZED_DATA_1(ht->buckets);

    hash = (*ht->hash_func)(key, ht->key_size) % ht->buckets_count;

    if (SLIST_COUNT(ht->buckets + hash) == 0) {
        goto new_entry;
    } else {
        SLIST_ITERATE(ht->buckets + hash, node)
        {
            entry = (struct ezi_hash_table_entry *)node->data;
            if (memcmp(entry->key, key, ht->key_size) == 0) {
                if (ht->free_func) {
                    (*ht->free_func)(entry->value);
                }
                memcpy(entry->value, value, ht->value_size);
                return 0;
            }
        }
    }

new_entry:
    if (!(entry = create_hash_table_entry(
              key, ht->key_size, value, ht->value_size))) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return -1;
    }

    if (ezi_slist_shift_no_alloc(ht->buckets + hash, (void *)entry) == 0) {
        ++ht->count;
        return 0;
    }

    return -1;
}

int
ezi_hash_table_remove(struct ezi_hash_table *ht, const void *key)
{
    uint32_t                     hash;
    struct ezi_hash_table_entry *entry;

    CHECK_NULL_PARAMS_2(ht, key);
    CHECK_UNINITIALIZED_DATA_1(ht->buckets);

    hash = (*ht->hash_func)(key, ht->key_size) % ht->buckets_count;

    if (ht->count == 0 || SLIST_COUNT(ht->buckets + hash) == 0) {
        errno = EZI_ERR_NOT_FOUND;
        return -1;
    }

    entry = (struct ezi_hash_table_entry *)ezi_slist_remove(
        ht->buckets + hash, key, (const void *)&ht->key_size, &compare_key);

    if (!entry) {
        return -1;
    }

    --ht->count;
    free_ezi_hash_table_entry(entry);
    free(entry);

    return 0;
}

void
ezi_hash_table_clear(struct ezi_hash_table *ht)
{
    int                    i;
    struct ezi_slist_node *node;

    for (i = 0; i < ht->buckets_count; ++i) {
        if (ht->free_func) {
            SLIST_ITERATE(ht->buckets + i, node)
            {
                (*ht->free_func)(
                    ((struct ezi_hash_table_entry *)node->data)->value);
            }
        }
        ezi_slist_clear(ht->buckets + i);
    }

    ht->count = 0;
}

void
free_ezi_hash_table(struct ezi_hash_table *ht)
{
    struct ezi_hash_table_entry *entry;
    struct ezi_slist_node *      node;

    node = NULL;

    while (ht->buckets_count--) {
        SLIST_ITERATE(ht->buckets + ht->buckets_count, node)
        {
            entry = (struct ezi_hash_table_entry *)node->data;
            free(entry->key);

            if (ht->free_func) {
                (*ht->free_func)(entry->value);
            }

            free(entry->value);
        }
        free_ezi_slist(ht->buckets + ht->buckets_count);
    }

    free(ht->buckets);
    memset(ht, 0, sizeof(*ht));
}

uint32_t
ezi_fnv1a_hash(const void *data, size_t len)
{
    uint32_t hash, i;

    for (i = 0, hash = FNV_OFFSET_BASIS; i < len; ++i) {
        hash ^= *(((uint8_t *)data) + i);
        hash *= FNV_PRIME;
    }

    return hash;
}

uint32_t
ezi_adler32_hash(const void *data, size_t len)
{
    uint32_t a, b, i;

    for (i = 0, a = 1, b = 0; i < len; ++i) {
        a = (a + *(((uint8_t *)data) + i)) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}

uint32_t
ezi_djb_hash(const void *data, size_t len)
{
    uint32_t hash, i;

    for (i = 0, hash = 5381; i < len; ++i) {
        hash = ((hash << 5) + hash) + *(((uint8_t *)data) + i);
    }

    return hash;
}

static struct ezi_hash_table_entry *
create_hash_table_entry(const void *key,
                        size_t      key_size,
                        const void *value,
                        size_t      value_size)
{
    struct ezi_hash_table_entry *entry;

    if (!(entry = (struct ezi_hash_table_entry *)malloc(sizeof(*entry)))) {
        return NULL;
    }

    if (!(entry->key = ezi_memdup(key, key_size))) {
        free(entry);
        return NULL;
    }

    if (!(entry->value = ezi_memdup(value, value_size))) {
        free(entry->key);
        free(entry);
        return NULL;
    }

    return entry;
}

static inline void
free_ezi_hash_table_entry(struct ezi_hash_table_entry *entry)
{
    free(entry->key);
    free(entry->value);
}

static int
compare_key(const void *k1, const void *k2, const void *sz)
{
    return memcmp(((struct ezi_hash_table_entry *)k1)->key, k2, *(size_t *)sz);
}
