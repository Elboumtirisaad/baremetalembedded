#include "circular_buffer.h"

#include <stdio.h>

static unsigned int checks = 0U;
static unsigned int failures = 0U;

#define CHECK(expr) do { checks++; if (!(expr)) { failures++; \
    (void)fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #expr); } } while (false)

static void test_validation(void)
{
    circular_buffer_t buffer;
    uint8_t storage[4U];
    CHECK(circular_buffer_init(NULL, storage, 4U) == CIRCULAR_BUFFER_NULL_ARGUMENT);
    CHECK(circular_buffer_init(&buffer, NULL, 4U) == CIRCULAR_BUFFER_NULL_ARGUMENT);
    CHECK(circular_buffer_init(&buffer, storage, 0U) == CIRCULAR_BUFFER_ZERO_CAPACITY);
    CHECK(circular_buffer_init(&buffer, storage, 4U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_is_valid(&buffer));
    CHECK(circular_buffer_is_empty(&buffer));
}

static void test_fifo_and_full_policy(void)
{
    circular_buffer_t buffer;
    uint8_t storage[3U];
    uint8_t value = 0U;
    CHECK(circular_buffer_init(&buffer, storage, 3U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 1U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 2U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 3U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 99U) == CIRCULAR_BUFFER_FULL);
    for (uint8_t expected = 1U; expected <= 3U; expected++)
    {
        CHECK(circular_buffer_pop(&buffer, &value) == CIRCULAR_BUFFER_OK);
        CHECK(value == expected);
    }
    CHECK(circular_buffer_pop(&buffer, &value) == CIRCULAR_BUFFER_EMPTY);
}

static void test_wrap_around(void)
{
    circular_buffer_t buffer;
    uint8_t storage[4U];
    uint8_t value = 0U;
    CHECK(circular_buffer_init(&buffer, storage, 4U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 1U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 2U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 3U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_pop(&buffer, &value) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_pop(&buffer, &value) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 4U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 5U) == CIRCULAR_BUFFER_OK);
    CHECK(circular_buffer_push(&buffer, 6U) == CIRCULAR_BUFFER_OK);
    for (uint8_t expected = 3U; expected <= 6U; expected++)
    {
        CHECK(circular_buffer_pop(&buffer, &value) == CIRCULAR_BUFFER_OK);
        CHECK(value == expected);
    }
}

static void test_corruption_detection(void)
{
    circular_buffer_t buffer;
    uint8_t storage[4U];
    CHECK(circular_buffer_init(&buffer, storage, 4U) == CIRCULAR_BUFFER_OK);
    buffer.count = 5U;
    CHECK(!circular_buffer_is_valid(&buffer));
    CHECK(circular_buffer_push(&buffer, 1U) == CIRCULAR_BUFFER_CORRUPT);
}

int main(void)
{
    test_validation();
    test_fifo_and_full_policy();
    test_wrap_around();
    test_corruption_detection();
    if (failures == 0U)
    {
        (void)printf("PASS: %u checks\n", checks);
        return 0;
    }
    (void)fprintf(stderr, "FAILED: %u of %u checks\n", failures, checks);
    return 1;
}
