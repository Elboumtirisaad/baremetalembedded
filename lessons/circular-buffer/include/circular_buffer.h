#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CIRCULAR_BUFFER_OK = 0,
    CIRCULAR_BUFFER_NULL_ARGUMENT,
    CIRCULAR_BUFFER_ZERO_CAPACITY,
    CIRCULAR_BUFFER_FULL,
    CIRCULAR_BUFFER_EMPTY,
    CIRCULAR_BUFFER_CORRUPT
} circular_buffer_status_t;

typedef struct
{
    uint8_t *storage;
    size_t capacity;
    size_t read_index;
    size_t write_index;
    size_t count;
} circular_buffer_t;

circular_buffer_status_t circular_buffer_init(circular_buffer_t *buffer,
                                              uint8_t *storage,
                                              size_t capacity);
circular_buffer_status_t circular_buffer_reset(circular_buffer_t *buffer);
circular_buffer_status_t circular_buffer_push(circular_buffer_t *buffer, uint8_t value);
circular_buffer_status_t circular_buffer_pop(circular_buffer_t *buffer, uint8_t *value);
circular_buffer_status_t circular_buffer_peek(const circular_buffer_t *buffer,
                                              uint8_t *value);
bool circular_buffer_is_valid(const circular_buffer_t *buffer);
size_t circular_buffer_size(const circular_buffer_t *buffer);
size_t circular_buffer_capacity(const circular_buffer_t *buffer);
bool circular_buffer_is_empty(const circular_buffer_t *buffer);
bool circular_buffer_is_full(const circular_buffer_t *buffer);

#ifdef __cplusplus
}
#endif

#endif
