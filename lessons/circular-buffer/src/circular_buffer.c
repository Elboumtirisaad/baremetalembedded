#include "circular_buffer.h"

static size_t advance_index(size_t index, size_t capacity)
{
    index++;
    if (index == capacity)
    {
        index = 0U;
    }
    return index;
}

static size_t expected_write_index(const circular_buffer_t *buffer)
{
    const size_t distance_to_end = buffer->capacity - buffer->read_index;
    if (buffer->count < distance_to_end)
    {
        return buffer->read_index + buffer->count;
    }
    return buffer->count - distance_to_end;
}

bool circular_buffer_is_valid(const circular_buffer_t *buffer)
{
    if ((buffer == NULL) || (buffer->storage == NULL) || (buffer->capacity == 0U))
    {
        return false;
    }
    if ((buffer->read_index >= buffer->capacity) ||
        (buffer->write_index >= buffer->capacity) ||
        (buffer->count > buffer->capacity))
    {
        return false;
    }
    return buffer->write_index == expected_write_index(buffer);
}

circular_buffer_status_t circular_buffer_init(circular_buffer_t *buffer,
                                              uint8_t *storage,
                                              size_t capacity)
{
    if ((buffer == NULL) || (storage == NULL))
    {
        return CIRCULAR_BUFFER_NULL_ARGUMENT;
    }
    if (capacity == 0U)
    {
        return CIRCULAR_BUFFER_ZERO_CAPACITY;
    }
    buffer->storage = storage;
    buffer->capacity = capacity;
    buffer->read_index = 0U;
    buffer->write_index = 0U;
    buffer->count = 0U;
    return CIRCULAR_BUFFER_OK;
}

circular_buffer_status_t circular_buffer_reset(circular_buffer_t *buffer)
{
    if (buffer == NULL)
    {
        return CIRCULAR_BUFFER_NULL_ARGUMENT;
    }
    if (!circular_buffer_is_valid(buffer))
    {
        return CIRCULAR_BUFFER_CORRUPT;
    }
    buffer->read_index = 0U;
    buffer->write_index = 0U;
    buffer->count = 0U;
    return CIRCULAR_BUFFER_OK;
}

circular_buffer_status_t circular_buffer_push(circular_buffer_t *buffer, uint8_t value)
{
    if (buffer == NULL)
    {
        return CIRCULAR_BUFFER_NULL_ARGUMENT;
    }
    if (!circular_buffer_is_valid(buffer))
    {
        return CIRCULAR_BUFFER_CORRUPT;
    }
    if (buffer->count == buffer->capacity)
    {
        return CIRCULAR_BUFFER_FULL;
    }
    buffer->storage[buffer->write_index] = value;
    buffer->write_index = advance_index(buffer->write_index, buffer->capacity);
    buffer->count++;
    return CIRCULAR_BUFFER_OK;
}

circular_buffer_status_t circular_buffer_pop(circular_buffer_t *buffer, uint8_t *value)
{
    if ((buffer == NULL) || (value == NULL))
    {
        return CIRCULAR_BUFFER_NULL_ARGUMENT;
    }
    if (!circular_buffer_is_valid(buffer))
    {
        return CIRCULAR_BUFFER_CORRUPT;
    }
    if (buffer->count == 0U)
    {
        return CIRCULAR_BUFFER_EMPTY;
    }
    *value = buffer->storage[buffer->read_index];
    buffer->read_index = advance_index(buffer->read_index, buffer->capacity);
    buffer->count--;
    return CIRCULAR_BUFFER_OK;
}

circular_buffer_status_t circular_buffer_peek(const circular_buffer_t *buffer,
                                              uint8_t *value)
{
    if ((buffer == NULL) || (value == NULL))
    {
        return CIRCULAR_BUFFER_NULL_ARGUMENT;
    }
    if (!circular_buffer_is_valid(buffer))
    {
        return CIRCULAR_BUFFER_CORRUPT;
    }
    if (buffer->count == 0U)
    {
        return CIRCULAR_BUFFER_EMPTY;
    }
    *value = buffer->storage[buffer->read_index];
    return CIRCULAR_BUFFER_OK;
}

size_t circular_buffer_size(const circular_buffer_t *buffer)
{
    return circular_buffer_is_valid(buffer) ? buffer->count : 0U;
}

size_t circular_buffer_capacity(const circular_buffer_t *buffer)
{
    return circular_buffer_is_valid(buffer) ? buffer->capacity : 0U;
}

bool circular_buffer_is_empty(const circular_buffer_t *buffer)
{
    return circular_buffer_is_valid(buffer) && (buffer->count == 0U);
}

bool circular_buffer_is_full(const circular_buffer_t *buffer)
{
    return circular_buffer_is_valid(buffer) && (buffer->count == buffer->capacity);
}
