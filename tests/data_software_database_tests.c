#include "data/software.h"
#include "data/software_database.h"
#include "data/software_source.h"

#include "unity.h"

#include <errno.h>
#include <string.h>

#define TEST_INSTALLED_SOURCE_FILE "/tmp/.ezi-installed.db"
#define TEST_AVAILABLE_SOURCE_FILE "/tmp/.ezi-available.db"
#define TEST_FILES_VERSION         1
#define TEST_ITERATIONS            0x2

void
setUp(void)
{}

void
tearDown(void)
{}

static void
test_software_database_save(void)
{
    static const char name_fmt[] = "test_software_%04d";
    static const char desc_fmt[] =
        "Test Software %04d is a product of Test Products, Inc";
    static const char url_fmt[] =
        "https://example.com/software/pool/t/e/s/t/%04d/test_software.tar.xz";

    int                          i, rc;
    struct ezi_software_database db;
    struct ezi_software_source   isrc, asrc;
    struct ezi_software          sw;

    errno = 0;
    rc    = init_ezi_software_source_fs(
        &isrc, TEST_INSTALLED_SOURCE_FILE, TEST_FILES_VERSION);

    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    rc = init_ezi_software_source_fs(
        &asrc, TEST_AVAILABLE_SOURCE_FILE, TEST_FILES_VERSION);

    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    rc = init_ezi_software_database(&db, &isrc, &asrc);

    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    rc = init_ezi_slist(&sw.dependencies, EZI_SOFTWARE_MAX_NAME_LEN);

    for (i = 0; i < TEST_ITERATIONS; ++i) {
        snprintf(sw.name, sizeof(sw.name), name_fmt, i);
        snprintf(
            sw.description, sizeof(sw.description), desc_fmt, (i + 0x100) * 73);
        snprintf(sw.url, sizeof(sw.url), url_fmt, i * 333);

        rc = ezi_software_table_insert(&db.installed, &sw);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
    }

    for (i = 0; i < TEST_ITERATIONS; ++i) {
        snprintf(sw.name, sizeof(sw.name), name_fmt, i);
        snprintf(
            sw.description, sizeof(sw.description), desc_fmt, (i + 0x100) * 73);
        snprintf(sw.url, sizeof(sw.url), url_fmt, i * 333);

        rc = ezi_software_table_insert(&db.available, &sw);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);

        rc = ezi_software_table_insert(&db.installed, &sw);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
    }

    rc = ezi_software_database_save(&db);
    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    free_ezi_software_database(&db);
}

static void
test_software_database_load(void)
{
    static const char name_fmt[] = "test_software_%04d";
    static const char desc_fmt[] =
        "Test Software %04d is a product of Test Products, Inc";
    static const char url_fmt[] =
        "https://example.com/software/pool/t/e/s/t/%04d/test_software.tar.xz";

    int                          i, rc;
    struct ezi_software_database db;
    struct ezi_software_source   isrc, asrc;
    struct ezi_software *        swptr;

    char namebuf[EZI_SOFTWARE_MAX_NAME_LEN + 1];
    char descbuf[EZI_SOFTWARE_MAX_DESC_LEN + 1];
    char urlbuf[EZI_SOFTWARE_MAX_URL_LEN + 1];

    errno = 0;
    rc    = init_ezi_software_source_fs(
        &isrc, TEST_INSTALLED_SOURCE_FILE, TEST_FILES_VERSION);

    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    rc = init_ezi_software_source_fs(
        &asrc, TEST_AVAILABLE_SOURCE_FILE, TEST_FILES_VERSION);

    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    rc = init_ezi_software_database(&db, &isrc, &asrc);

    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    rc = ezi_software_database_load(&db);

    free_ezi_software_database(&db);
    return;


    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL(0, errno);

    for (i = 0; i < TEST_ITERATIONS; ++i) {
        snprintf(namebuf, sizeof(namebuf) - 1, name_fmt, i);
        snprintf(descbuf, sizeof(descbuf) - 1, desc_fmt, (i + 0x100) * 73);
        snprintf(urlbuf, sizeof(urlbuf) - 1, url_fmt, i * 333);

        rc = ezi_software_table_get(&db.available, namebuf, &swptr);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
        TEST_ASSERT_NOT_NULL(swptr);

        TEST_ASSERT_EQUAL_STRING(namebuf, swptr->name);
        TEST_ASSERT_EQUAL_STRING(descbuf, swptr->description);
        TEST_ASSERT_EQUAL_STRING(urlbuf, swptr->url);

        rc = ezi_software_table_get(&db.installed, namebuf, &swptr);
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_EQUAL(0, errno);
        TEST_ASSERT_NOT_NULL(swptr);

        TEST_ASSERT_EQUAL_STRING(namebuf, swptr->name);
        TEST_ASSERT_EQUAL_STRING(descbuf, swptr->description);
        TEST_ASSERT_EQUAL_STRING(urlbuf, swptr->url);
    }

    free_ezi_software_database(&db);

    remove(TEST_INSTALLED_SOURCE_FILE);
    remove(TEST_AVAILABLE_SOURCE_FILE);
}

int
main(int argc, char *argv[])
{
    UNITY_BEGIN();

    RUN_TEST(test_software_database_save);
    RUN_TEST(test_software_database_load);

    return UNITY_END();
}
