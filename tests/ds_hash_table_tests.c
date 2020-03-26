#include "ds/hash_table.h"
#include "log/logger.h"

#include "unity.h"

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define TEST_BUCKETS_COUNT 0x1000
#define TEST_ITERATIONS    0x1000

#define TEST_TYPE       size_t
#define TEST_KEY_TYPE   size_t
#define TEST_VALUE_TYPE size_t

struct ezi_hash_table ht;

void
setUp(void)
{}

void
tearDown(void)
{}

static void
test_hash_table_initialization(void)
{
    int rc;

    errno = 0;

    rc = init_ezi_hash_table(
        &ht, TEST_BUCKETS_COUNT, sizeof(char), sizeof(TEST_TYPE));
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);
    TEST_ASSERT_EQUAL(sizeof(char), ht.key_size);
    TEST_ASSERT_EQUAL(sizeof(TEST_TYPE), ht.value_size);
    TEST_ASSERT_EQUAL(0, ht.count);
    TEST_ASSERT_EQUAL(TEST_BUCKETS_COUNT, ht.buckets_count);
    TEST_ASSERT_NOT_NULL(ht.buckets);

    free_ezi_hash_table(&ht);

    rc = init_ezi_hash_table(
        &ht, TEST_BUCKETS_COUNT, sizeof(double), sizeof(long long));
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);
    TEST_ASSERT_EQUAL(sizeof(double), ht.key_size);
    TEST_ASSERT_EQUAL(sizeof(long long), ht.value_size);
    TEST_ASSERT_EQUAL(0, ht.count);
    TEST_ASSERT_EQUAL(TEST_BUCKETS_COUNT, ht.buckets_count);
    TEST_ASSERT_NOT_NULL(ht.buckets);

    free_ezi_hash_table(&ht);

    rc = init_ezi_hash_table(
        &ht, TEST_BUCKETS_COUNT, sizeof(uint32_t), sizeof(char[0x100]));
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);
    TEST_ASSERT_EQUAL(sizeof(uint32_t), ht.key_size);
    TEST_ASSERT_EQUAL(sizeof(char[0x100]), ht.value_size);
    TEST_ASSERT_EQUAL(0, ht.count);
    TEST_ASSERT_EQUAL(TEST_BUCKETS_COUNT, ht.buckets_count);
    TEST_ASSERT_NOT_NULL(ht.buckets);

    free_ezi_hash_table(&ht);
}

static void
test_hash_table(void)
{
    TEST_TYPE key, value, *ret_val;
    int       rc;

    errno = 0;
    rc    = init_ezi_hash_table(&ht,
                             TEST_BUCKETS_COUNT,
                             sizeof(TEST_KEY_TYPE),
                             sizeof(TEST_VALUE_TYPE));
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    for (key = 1; key <= TEST_ITERATIONS; ++key) {
        value = rand();
        /* Add an item */
        rc = ezi_hash_table_set(&ht, &key, &value);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
        TEST_ASSERT_EQUAL(key, ht.count);

        /* Check the item existance */
        rc = ezi_hash_table_has_key(&ht, &key);
        TEST_ASSERT_EQUAL(1, rc);
        TEST_ASSERT_EQUAL(0, errno);

        /* Check the stored value */
        rc = ezi_hash_table_get(&ht, &key, (void **)&ret_val);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
        TEST_ASSERT_NOT_NULL(ret_val);
        TEST_ASSERT_EQUAL(value, (void *)*ret_val);

        /* Remove the item */
        rc = ezi_hash_table_remove(&ht, &key);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
        TEST_ASSERT_EQUAL(key - 1, ht.count);

        /* Check the item existance */
        rc = ezi_hash_table_has_key(&ht, &key);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);

        /* Re-add the item */
        rc = ezi_hash_table_set(&ht, &key, &value);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
        TEST_ASSERT_EQUAL(key, ht.count);

        /* Check the item existance */
        rc = ezi_hash_table_has_key(&ht, &key);
        TEST_ASSERT_EQUAL(1, rc);
        TEST_ASSERT_EQUAL(0, errno);

        /* Check the stored value */
        rc = ezi_hash_table_get(&ht, &key, (void **)&ret_val);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
        TEST_ASSERT_NOT_NULL(ret_val);
        TEST_ASSERT_EQUAL(value, *ret_val);
    }

    free_ezi_hash_table(&ht);
}

int
main(int argc, char *argv[])
{
    UNITY_BEGIN();

    RUN_TEST(&test_hash_table_initialization);
    RUN_TEST(&test_hash_table);

    return UNITY_END();
}
