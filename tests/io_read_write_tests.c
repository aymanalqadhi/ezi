#include "io/read_write.h"

#include "unity.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define TEST_BUF_SIZE                                                          \
    (0x1000 * (sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint64_t)))
#define STRING_TEST_ITERATIONS 0x100

#define ARRLEN(arr) (sizeof(arr) / sizeof(arr[0]))

void
setUp(void)
{}

void
tearDown(void)
{}

static void
test_uintxx_read_write(void)
{
    int i;

    uint16_t num16, temp16;
    uint32_t num32, temp32;
    uint64_t num64, temp64;

    char    buf[TEST_BUF_SIZE];
    ssize_t total_len;
    FILE *  fp;

    TEST_ASSERT_NOT_NULL(fp = fmemopen(buf, ARRLEN(buf), "w+"));

    for (total_len = TEST_BUF_SIZE, num16 = num32 = num64 = 0; total_len > 0;
         num16 += 0x1000, num32 += 0x2000, num64 += 0x4000) {
        TEST_ASSERT_EQUAL(0, ezi_fs_write_uint16(num16, fp));
        TEST_ASSERT_EQUAL(0, ezi_fs_write_uint32(num32, fp));
        TEST_ASSERT_EQUAL(0, ezi_fs_write_uint64(num64, fp));

        total_len -= sizeof(uint16_t);
        total_len -= sizeof(uint32_t);
        total_len -= sizeof(uint64_t);
    }

    TEST_ASSERT_EQUAL(0, fseek(fp, 0, SEEK_SET));

    for (total_len = TEST_BUF_SIZE, num16 = num32 = num64 = 0; total_len > 0;
         num16 += 0x1000, num32 += 0x2000, num64 += 0x4000) {
        TEST_ASSERT_EQUAL(0, ezi_fs_read_uint16(&temp16, fp));
        TEST_ASSERT_EQUAL(num16, temp16);

        TEST_ASSERT_EQUAL(0, ezi_fs_read_uint32(&temp32, fp));
        TEST_ASSERT_EQUAL(num32, temp32);

        TEST_ASSERT_EQUAL(0, ezi_fs_read_uint64(&temp64, fp));
        TEST_ASSERT_EQUAL(num64, temp64);

        total_len -= sizeof(uint16_t);
        total_len -= sizeof(uint32_t);
        total_len -= sizeof(uint64_t);
    }
}

static void
test_string_read_write(void)
{
    int   i;
    FILE *fp;
    char  str[] =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi "
        "suscipit ex a velit consequat, nec condimentum nunc pellentesque. "
        "Curabitur viverra mattis velit sit amet suscipit. Nam non nunc "
        "tellus. Curabitur tempor turpis lacinia ipsum cursus, ac porttitor "
        "lorem laoreet. Sed elementum, mauris sed fringilla tempor, nibh "
        "libero molestie urna, eget faucibus elit magna eu augue. Pellentesque "
        "sodales erat viverra sagittis egestas. Maecenas interdum dolor et "
        "diam egestas varius. Vestibulum varius tellus id tincidunt fermentum. "
        "Suspendisse potenti. Ut sed libero velit. Nam venenatis, sem ut "
        "facilisis faucibus, sem elit bibendum mauris, ac vestibulum sapien "
        "ipsum at mi. Aliquam sed tellus et odio mollis suscipit vel eget "
        "ligula. In dignissim, lorem quis lobortis tempor, purus nisi "
        "elementum felis, id malesuada mi sapien a ante. Nunc at risus "
        "ultrices diam elementum molestie vitae at neque.";

    char   buf[ARRLEN(str) * STRING_TEST_ITERATIONS +
             sizeof(uint32_t) * STRING_TEST_ITERATIONS];
    char   tmp[ARRLEN(str)];
    size_t len;

    TEST_ASSERT_NOT_NULL(fp = fmemopen(buf, ARRLEN(buf), "w+"));

    for (i = 0; i < STRING_TEST_ITERATIONS; ++i) {
        TEST_ASSERT_EQUAL(0, ezi_fs_write_string(str, fp));
    }

    TEST_ASSERT_EQUAL(0, fseek(fp, 0, SEEK_SET));

    for (i = 0; i < STRING_TEST_ITERATIONS; ++i) {
        len = strlen(str);
        TEST_ASSERT_EQUAL(0, ezi_fs_read_string(tmp, &len, fp));
        TEST_ASSERT_EQUAL(len, strlen(str));
        TEST_ASSERT_EQUAL_STRING(str, tmp);
    }

    TEST_ASSERT_EQUAL(-1, ezi_fs_read_string(tmp, &len, fp));
}

int
main(int argc, char *argv[])
{
    UNITY_BEGIN();

    RUN_TEST(test_uintxx_read_write);
    RUN_TEST(test_string_read_write);

    return UNITY_END();
}
