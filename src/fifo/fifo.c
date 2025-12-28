/**
 * @file fifo.c
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief FIFO Implementation
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "fifo.h"


void fifo_init(FIFO_t *fifo, fifo_data_t *buffer, uint32_t size)
{
    fifo->buffer = buffer;
    fifo->size = size;
    fifo->head = 0;
    fifo->tail = 0;
}

bool fifo_push(FIFO_t *fifo, fifo_data_t value)
{
    uint32_t next_head = (fifo->head + 1);
    if (next_head >= fifo->size)
    {
        next_head = 0;
    }

    if (next_head == fifo->tail)
    {
        return false; // Full
    }

    fifo->buffer[fifo->head] = value;
    fifo->head = next_head;
    return true;
}

bool fifo_pop(FIFO_t *fifo, fifo_data_t *value)
{
    if (fifo->head == fifo->tail)
    {
        return false; // Empty
    }

    *value = fifo->buffer[fifo->tail];
    
    uint32_t next_tail = (fifo->tail + 1);
    if (next_tail >= fifo->size)
    {
        next_tail = 0;
    }
    
    fifo->tail = next_tail;
    return true;
}

bool fifo_is_empty(FIFO_t *fifo)
{
    return (fifo->head == fifo->tail);
}

bool fifo_is_full(FIFO_t *fifo)
{
    uint32_t next_head = (fifo->head + 1);
    if (next_head >= fifo->size)
    {
        next_head = 0;
    }
    return (next_head == fifo->tail);
}

uint32_t fifo_count(FIFO_t *fifo)
{
    if (fifo->head >= fifo->tail)
    {
        return fifo->head - fifo->tail;
    } else
    {
        return fifo->size + fifo->head - fifo->tail;
    }
}
