#include "data/software.h"
#include "data/software_source.h"
#include "ds/hash_table.h"
#include "log/errno.h"

#include "unity.h"

#include <errno.h>
#include <string.h>

#define TEST_DATA_BUCKETS            0x1000
#define TEST_DATA_SOURCE_FILE        "/tmp/.ezi_software_data_source"
#define TEST_DATA_SOURCE_VERSION     0x7E57
#define TEST_DATA_SOURCE_LAST_UPDATE 0xABCDEF
#define TEST_DATA_SOURCE_CHECKSUM    0x12345678
#define TEST_ITERATIONS              0x1000

void
setUp(void)
{}

void
tearDown(void)
{}

static void
test_software_souce(void)
{
    static const char name_fmt[] = "test_software_%04d";
    static const char desc_fmt[] =
        "Test Software %04d is a product of Test Products, Inc";
    static const char url_fmt[] =
        "https://example.com/software/pool/t/e/s/t/%04d/test_software.tar.xz";

    int                        i, rc;
    struct ezi_software_source src1, src2;
    struct ezi_hash_table      data;
    struct ezi_software        sw, *swptr;
    char                       namebuf[EZI_SOFTWARE_MAX_NAME_LEN + 1],
        descbuf[EZI_SOFTWARE_MAX_DESC_LEN + 1],
        urlbuf[EZI_SOFTWARE_MAX_URL_LEN + 1];

    errno = 0;

    rc = init_ezi_hash_table(&data,
                             TEST_DATA_BUCKETS,
                             EZI_SOFTWARE_MAX_NAME_LEN,
                             sizeof(struct ezi_software));
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    rc = init_ezi_software_source_fs(
        &src1, TEST_DATA_SOURCE_FILE, TEST_DATA_SOURCE_VERSION);

    TEST_ASSERT_NOT_NULL(src1.source_fp);
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);
    TEST_ASSERT_EQUAL(TEST_DATA_SOURCE_VERSION, src1.metadata.version);

    src1.metadata.checksum    = TEST_DATA_SOURCE_CHECKSUM;
    src1.metadata.last_update = TEST_DATA_SOURCE_LAST_UPDATE;

    TEST_ASSERT_EQUAL(
        0, init_ezi_slist(&sw.dependencies, EZI_SOFTWARE_MAX_NAME_LEN));

    for (i = 0; i < TEST_ITERATIONS; ++i) {
        memset(sw.name, 0, sizeof(sw.name));
        snprintf(sw.name, sizeof(sw.name), name_fmt, i);
        snprintf(
            sw.description, sizeof(sw.description), desc_fmt, (i + 0x100) * 73);
        snprintf(sw.url, sizeof(sw.url), url_fmt, i * 333);

        rc = ezi_hash_table_set(&data, sw.name, &sw);

        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
    }

    rc = ezi_software_source_save(&src1, &data);
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    free_ezi_software_source(&src1);
    free_ezi_hash_table(&data);

    rc = init_ezi_software_source_fs(&src2, TEST_DATA_SOURCE_FILE, 0);

    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    rc = init_ezi_hash_table(&data,
                             TEST_DATA_BUCKETS,
                             EZI_SOFTWARE_MAX_NAME_LEN,
                             sizeof(struct ezi_software));
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    rc = ezi_software_source_load(&src2, &data);
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    for (i = 0; i < TEST_ITERATIONS; ++i) {
        memset(namebuf, 0, sizeof(namebuf));
        snprintf(namebuf, sizeof(namebuf) - 1, name_fmt, i);
        snprintf(descbuf, sizeof(descbuf) - 1, desc_fmt, (i + 0x100) * 73);
        snprintf(urlbuf, sizeof(urlbuf) - 1, url_fmt, i * 333);

        rc = ezi_hash_table_get(&data, (const void *)namebuf, (void **)&swptr);

        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);

        TEST_ASSERT_EQUAL_STRING(namebuf, swptr->name);
        TEST_ASSERT_EQUAL_STRING(descbuf, swptr->description);
        TEST_ASSERT_EQUAL_STRING(urlbuf, swptr->url);
    }

    free_ezi_software_source(&src2);
    free_ezi_hash_table(&data);

    remove(TEST_DATA_SOURCE_FILE);
}

int
main(int argc, char *argv[])
{
    UNITY_BEGIN();

    RUN_TEST(test_software_souce);

    return UNITY_END();
}
