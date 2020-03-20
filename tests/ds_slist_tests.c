#include <stddef.h>
#include <stdio.h>

#include "ds/slist.h"

#include "unity.h"

#define TEST_ITERATIONS 0x10000
#define TEST_TYPE       size_t

static struct ezi_slist sl;

void
setUp(void)
{}
void
tearDown(void)
{
    free_ezi_slist(&sl);
}

static void
test_slist_initialization(void)
{
    TEST_ASSERT_EQUAL(0, init_ezi_slist(&sl, sizeof(short)));
    TEST_ASSERT_EQUAL(sizeof(short), sl.__element_size);
    TEST_ASSERT_EQUAL(0, sl.count);
    TEST_ASSERT_NULL(sl.head);
    TEST_ASSERT_NULL(sl.tail);

    TEST_ASSERT_EQUAL(0, init_ezi_slist(&sl, sizeof(int)));
    TEST_ASSERT_EQUAL(sizeof(int), sl.__element_size);
    TEST_ASSERT_EQUAL(0, sl.count);
    TEST_ASSERT_NULL(sl.head);
    TEST_ASSERT_NULL(sl.tail);

    TEST_ASSERT_EQUAL(0, init_ezi_slist(&sl, sizeof(double)));
    TEST_ASSERT_EQUAL(sizeof(double), sl.__element_size);
    TEST_ASSERT_EQUAL(0, sl.count);
    TEST_ASSERT_NULL(sl.head);
    TEST_ASSERT_NULL(sl.tail);
}

static void
test_slist_push(void)
{
    TEST_TYPE i;

    TEST_ASSERT_EQUAL(0, init_ezi_slist(&sl, sizeof(i)));

    for (i = 1; i <= TEST_ITERATIONS; ++i) {
        ezi_slist_push(&sl, &i);
        TEST_ASSERT_EQUAL(i, sl.count);
        TEST_ASSERT_EQUAL(i, *SLIST_LAST(&sl, TEST_TYPE *));
    }

    TEST_ASSERT_EQUAL(TEST_ITERATIONS, sl.count);
}

static void
test_slist_pop(void)
{
    TEST_TYPE              i, *val;
    struct ezi_slist_node *node;

    for (i = sl.count; i > 0; --i) {
        TEST_ASSERT_EQUAL(i, *SLIST_LAST(&sl, TEST_TYPE *));
        node = ezi_slist_pop(&sl);
        TEST_ASSERT_NOT_NULL(node);
        TEST_ASSERT_EQUAL(*(TEST_TYPE *)node->data, i);
        free_ezi_slist_node(node);
    }

    TEST_ASSERT(sl.head == NULL && sl.tail == NULL);
}

static void
test_slist_shift(void)
{
    TEST_TYPE i;

    TEST_ASSERT_EQUAL(0, sl.count);
    TEST_ASSERT_EQUAL(0, init_ezi_slist(&sl, sizeof(i)));

    for (i = 1; i <= TEST_ITERATIONS; ++i) {
        ezi_slist_shift(&sl, &i);
        TEST_ASSERT_EQUAL(i, sl.count);
        TEST_ASSERT_EQUAL(i, *SLIST_FIRST(&sl, TEST_TYPE *));
    }

    TEST_ASSERT_EQUAL(TEST_ITERATIONS, sl.count);
}

static void
test_slist_unshift(void)
{
    TEST_TYPE              i, *val;
    struct ezi_slist_node *node;

    for (i = sl.count; i > 0; --i) {
        TEST_ASSERT_EQUAL(i, *SLIST_FIRST(&sl, TEST_TYPE *));
        node = ezi_slist_unshift(&sl);
        TEST_ASSERT_NOT_NULL(node);
        TEST_ASSERT_EQUAL(*(TEST_TYPE *)node->data, i);
        free_ezi_slist_node(node);
    }

    TEST_ASSERT(sl.head == NULL && sl.tail == NULL);
}

int
main(int argc, char *argv[])
{
    UNITY_BEGIN();

    RUN_TEST(test_slist_initialization);
    RUN_TEST(test_slist_push);
    RUN_TEST(test_slist_pop);
    RUN_TEST(test_slist_shift);
    RUN_TEST(test_slist_unshift);

    return UNITY_END();
}
