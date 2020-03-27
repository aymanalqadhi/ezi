#include "data/software.h"
#include "ds/slist.h"

#include "unity.h"

#include <string.h>

#define TEST_BUF_LEN     0x100000
#define TEST_STR_BUF_LEN 0x100
#define TEST_ITERATIONS  0x2

void
setUp(void)
{}

void
tearDown(void)
{}

static void
test_software_encode_decode(void)
{
    static const char name_fmt[] = "test_software_%04d";
    static const char desc_fmt[] =
        "Test Software %04d is a product of Test Products, Inc";
    static const char url_fmt[] =
        "https://example.com/software/pool/t/e/s/t/%04d/test_software.tar.xz";

    int                 i;
    char                strbuf[TEST_STR_BUF_LEN];
    uint8_t             buf[TEST_BUF_LEN];
    size_t              encoded, decoded, total;
    struct ezi_software sw, dsw;

    TEST_ASSERT_EQUAL(
        0, init_ezi_slist(&sw.dependencies, EZI_SOFTWARE_MAX_NAME_LEN));

    for (i = 0, encoded = 0; i < TEST_ITERATIONS; ++i) {
        total = sizeof(buf) - encoded;

        snprintf(sw.name, sizeof(sw.name), name_fmt, i);
        snprintf(
            sw.description, sizeof(sw.description), desc_fmt, (i + 0x100) * 73);
        snprintf(sw.url, sizeof(sw.url), url_fmt, i * 333);

        TEST_ASSERT_EQUAL(
            0, ezi_software_encode(&sw, (void *)(buf + encoded), &total));

        encoded += total;
    }

    for (i = 0, decoded = 0; i < TEST_ITERATIONS; ++i) {
        total = encoded - decoded;

        TEST_ASSERT_EQUAL(
            0, ezi_software_decode(&sw, (void *)(buf + decoded), &total));
        snprintf(strbuf, sizeof(strbuf), name_fmt, i);
        TEST_ASSERT_EQUAL_STRING(sw.name, strbuf);
        snprintf(strbuf, sizeof(strbuf), desc_fmt, (i + 0x100) * 73);
        TEST_ASSERT_EQUAL_STRING(sw.description, strbuf);
        snprintf(strbuf, sizeof(strbuf), url_fmt, i * 333);
        TEST_ASSERT_EQUAL_STRING(sw.url, strbuf);

        decoded += total;
    }

    TEST_ASSERT_EQUAL(encoded, decoded);
}

int
main(int argc, char *argv[])
{
    UNITY_BEGIN();

    RUN_TEST(test_software_encode_decode);

    return UNITY_END();
}
