#include "io/endian.h"

#include "unity.h"

#include <stddef.h>
#include <stdint.h>

static const uint16_t num16 = 0xAABB;
static const uint32_t num32 = 0xAABBCCDD;
static const uint64_t num64 = 0xAABBCCDDEEFF0011;

void
setUp(void)
{}

void
tearDown(void)
{}

static void
test_unsigned_integer_sizes(void)
{
    TEST_ASSERT_EQUAL(sizeof(uint16_t), 2);
    TEST_ASSERT_EQUAL(sizeof(uint32_t), 4);
    TEST_ASSERT_EQUAL(sizeof(uint64_t), 8);
}

static void
test_is_big_endian(void)
{
    if (is_big_endian()) {
        TEST_ASSERT_EQUAL(*(uint8_t *)(&num16 + 1), num16 & 0xFF);
        TEST_ASSERT_EQUAL(*(uint8_t *)(&num32 + 3), num32 & 0xFF);
        TEST_ASSERT_EQUAL(*(uint8_t *)(&num64 + 7), num64 & 0xFF);
    } else {
        TEST_ASSERT_EQUAL(*(uint8_t *)&num16, num16 & 0xFF);
        TEST_ASSERT_EQUAL(*(uint8_t *)&num32, num32 & 0xFF);
        TEST_ASSERT_EQUAL(*(uint8_t *)&num64, num64 & 0xFF);
    }
}

static void
test_swap_endianness(void)
{
    TEST_ASSERT_EQUAL(num16 & 0xFF, 0xBB);
    TEST_ASSERT_EQUAL(ezi_swap_endianness_16(num16) & 0xFF, 0xAA);
    TEST_ASSERT_EQUAL(num32 & 0xFF, 0xDD);
    TEST_ASSERT_EQUAL(ezi_swap_endianness_32(num32) & 0xFF, 0xAA);
    TEST_ASSERT_EQUAL(num64 & 0xFF, 0x11);
    TEST_ASSERT_EQUAL(ezi_swap_endianness_64(num64) & 0xFF, 0xAA);

    TEST_ASSERT_EQUAL(0xBBAA, ezi_swap_endianness_16(num16));
    TEST_ASSERT_EQUAL(0xDDCCBBAA, ezi_swap_endianness_32(num32));
    TEST_ASSERT_EQUAL(0x1100FFEEDDCCBBAA, ezi_swap_endianness_64(num64));
}

static void
test_host_to_big_endian(void)
{
    if (is_big_endian()) {
        TEST_ASSERT_EQUAL(num16, ezi_htobe16(num16));
        TEST_ASSERT_EQUAL(num32, ezi_htobe32(num32));
        TEST_ASSERT_EQUAL(num64, ezi_htobe64(num64));
    } else {
        TEST_ASSERT_EQUAL(ezi_swap_endianness_16(num16), ezi_htobe16(num16));
        TEST_ASSERT_EQUAL(ezi_swap_endianness_32(num32), ezi_htobe32(num32));
        TEST_ASSERT_EQUAL(ezi_swap_endianness_64(num64), ezi_htobe64(num64));
    }
}

static void
test_big_endian_to_host(void)
{
    if (is_big_endian()) {
        TEST_ASSERT_EQUAL(num16, ezi_betoh16(num16));
        TEST_ASSERT_EQUAL(num32, ezi_betoh32(num32));
        TEST_ASSERT_EQUAL(num64, ezi_betoh64(num64));
    } else {
        TEST_ASSERT_EQUAL(ezi_swap_endianness_16(num16), ezi_betoh16(num16));
        TEST_ASSERT_EQUAL(ezi_swap_endianness_32(num32), ezi_betoh32(num32));
        TEST_ASSERT_EQUAL(ezi_swap_endianness_64(num64), ezi_betoh64(num64));
    }
}

int
main(int argc, char *argv[])
{
    UNITY_BEGIN();

    RUN_TEST(test_unsigned_integer_sizes);
    RUN_TEST(test_is_big_endian);
    RUN_TEST(test_swap_endianness);
    RUN_TEST(test_big_endian_to_host);
    RUN_TEST(test_host_to_big_endian);

    return UNITY_END();
}
