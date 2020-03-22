#ifndef EZI_DS_HASH_TABLE_H
#define EZI_DS_HASH_TABLE_H

#include "ds/slist.h"

#include <stddef.h>
#include <stdint.h>

#define DEFAULT_HASH_FUNCTION      ezi_fnv1a_hash
#define DEFAULT_HASH_TABLE_BUCKETS 0x40

typedef uint32_t (*hash_func)(const void *, size_t);

struct ezi_hash_table_entry
{
    void *key;
    void *value;
};

struct ezi_hash_table
{
    size_t count;
    size_t buckets_count;
    size_t key_size;
    size_t value_size;

    struct ezi_slist *buckets;
    hash_func         hash;
};

/*!
 * \brief Initializes a generic hash table with a spcific element size
 *
 * \param [in,out] ht            A pointer to the hash table to initialize
 * \param [in]     buckets_count The hash table buckets count
 * \param [in]     key_size      The size of the key of each entry
 * \param [in]     value_size    The size of the value of each entry
 * \return 0 on success, -1 otherwise with errno set
 */
int
init_ezi_hash_table(struct ezi_hash_table *ht,
                    size_t                 buckets_count,
                    size_t                 key_size,
                    size_t                 value_size);

/*!
 * \brief Gets a value of an entry in the hash table by key
 *
 * \param [in]  ht        A pointer to the hash table
 * \param [in]  key       A pointer to the key to be used
 * \param [out] value_ptr A pointer to the output pointer
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_hash_table_get(struct ezi_hash_table *ht,
                   const void *           key,
                   void **                value_ptr);

/*!
 * \brief Checks whether the hash table has a particular key
 *
 * \param [in] ht    A pointer to hash table to check in
 * \parap [in] key   A pointer to the key to be checked
 * \return 1 If found, 0 if not, -1 for error with errno set
 */
int
ezi_hash_table_has_key(struct ezi_hash_table *ht, const void *key);

/*!
 * \brief Sets a value in the hash table with a specific key
 *
 * \param [in]  ht    A pointer to the hash table to set the value into
 * \param [in]  key   A pointer to the key to use
 * \param [in]  value A pointer to the value to use (Can be NULL)
 * \return 0 on success, 1 if not found, -1 otherwise with errno set
 */
int
ezi_hash_table_set(struct ezi_hash_table *ht,
                   const void *           key,
                   const void *           value);

/*!
 * \brief Removes an item with a key
 *
 * \param [in] ht   A pointer to the hash table which to remove the item from
 * \param [in] key  A pointer to the key of the item to be removed
 * \return 0 for success, -1 if the item was not found or error with errno set
 */
int
ezi_hash_table_remove(struct ezi_hash_table *ht, const void *key);

/*!
 * \brief Frees the resources used by the hash table
 *
 * \param [in]  ht  A pointer to the hash table
 */
void
free_ezi_hash_table(struct ezi_hash_table *ht);

/*!
 * \briref Glenn Fowler, Phong Vo, and Landon Curt hash function
 *
 * \param [in] data  The data to hash
 * \param [in] len   The length of the data
 * \return The computed hash
 */
uint32_t
ezi_fnv1a_hash(const void *data, size_t len);

/*!
 * \briref Mark Adler hash function
 *
 * \param [in] data  The data to hash
 * \param [in] len   The length of the data
 * \return The computed hash
 */
uint32_t
ezi_adler32_hash(const void *data, size_t len);

/*!
 * \briref Dan J. Bernstein hash unction
 *
 * \param [in] data  The data to hash
 * \param [in] len   The length of the data
 * \return The computed hash
 */
uint32_t
ezi_djb_hash(const void *data, size_t len);

#endif /* EZI_DS_HASH_TABLE_H */
