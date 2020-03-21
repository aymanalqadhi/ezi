#include "ds/hash_table.h"
#include "ds/slist.h"

#include "log/errno.h"
#include "util/memory.h"

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
                        size_t      value_size)
{
    struct ezi_hash_table_entry *entry;

    if ((entry = (struct ezi_hash_table_entry *)malloc(sizeof(*entry))) ==
        NULL) {
        return NULL;
    }

    if ((entry->key = ezi_memdup(key, key_size)) == NULL) {
        free(entry);
        return NULL;
    }

    if ((entry->value = ezi_memdup(value, value_size)) == NULL) {
        free(entry->key);
        free(entry);
        return NULL;
    }

    return entry;
}

int
init_ezi_hash_table(struct ezi_hash_table *ht,
                    size_t                 buckets_count,
                    size_t                 key_size,
                    size_t                 value_size)
{
    int i;

    if (ht == NULL) {
        errno = EZI_ERR_NULL_ARGUMENTS;
        return -1;
    }

    buckets_count = buckets_count > 0 ? buckets_count
                                      : DEFAULT_HASH_TABLE_BUCKETS;

    if ((ht->buckets = calloc(ht->buckets_count = buckets_count,
                              sizeof(*ht->buckets))) == NULL) {
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

    ht->count      = 0;
    ht->key_size   = key_size;
    ht->value_size = value_size;
    ht->hash       = DEFAULT_HASH_FUNCTION;

    return 0;
}

int
ezi_hash_table_get(struct ezi_hash_table *ht, const void *key, void **value_ptr)
{
    size_t                       hash;
    struct ezi_hash_table_entry *entry;
    struct ezi_slist_node *      node;

    if (ht == NULL || key == NULL || value_ptr == NULL) {
        errno = EZI_ERR_NULL_ARGUMENTS;
        return -1;
    } else if (ht->buckets == NULL) {
        errno = EZI_ERR_UNINITIALIZED_DATA;
        return -1;
    } else if (ht->count == 0) {
        *value_ptr = NULL;
        return 0;
    }

    hash = (ht->hash)(key, ht->key_size) % ht->buckets_count;

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
ezi_hash_table_set(struct ezi_hash_table *ht,
                   const void *           key,
                   const void *           value)
{
    size_t                       hash;
    struct ezi_hash_table_entry *entry;
    struct ezi_slist_node *      node;

    if (ht == NULL || key == NULL || value == NULL) {
        errno = EZI_ERR_NULL_ARGUMENTS;
        return -1;
    } else if (ht->buckets == NULL) {
        errno = EZI_ERR_UNINITIALIZED_DATA;
        return -1;
    }

    hash = (*ht->hash)(key, ht->key_size) % ht->buckets_count;

    if (SLIST_COUNT(ht->buckets + hash) == 0) {
        goto new_entry;
    } else {
        SLIST_ITERATE(ht->buckets + hash, node)
        {
            entry = (struct ezi_hash_table_entry *)node->data;
            if (memcmp(entry->key, key, ht->key_size) == 0) {
                memcpy(entry->value, value, ht->value_size);
                return 0;
            }
        }
    }

new_entry:
    if ((entry = create_hash_table_entry(
             key, ht->key_size, value, ht->value_size)) == NULL) {
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
ezi_hash_table_has_key(struct ezi_hash_table *ht, const void *key)
{
    size_t                       hash;
    struct ezi_hash_table_entry *entry;
    struct ezi_slist_node *      node;

    if (ht == NULL || key == NULL) {
        errno = EZI_ERR_NULL_ARGUMENTS;
        return -1;
    } else if (ht->buckets == NULL) {
        errno = EZI_ERR_UNINITIALIZED_DATA;
        return -1;
    }

    hash = (*ht->hash)(key, ht->key_size) % ht->buckets_count;

    SLIST_ITERATE(ht->buckets + hash, node)
    {
        entry = (struct ezi_hash_table_entry *)node->data;
        if (memcmp(key, entry->key, ht->key_size) == 0) {
            return 1;
        }
    }

    return 0;
}

void
free_hash_table(struct ezi_hash_table *ht)
{
    struct ezi_hash_table_entry *entry;
    struct ezi_slist_node *      node;

    node = NULL;

    while (ht->buckets_count--) {
        SLIST_ITERATE(ht->buckets + ht->buckets_count, node)
        {
            entry = (struct ezi_hash_table_entry *)node->data;
            free(entry->key);
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
