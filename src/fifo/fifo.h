#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include <stdbool.h>
#include "arm_math.h" 

typedef float32_t fifo_data_t; // Define data type for FIFO

typedef struct {
    fifo_data_t *buffer;
    uint32_t size;
    volatile uint32_t head;
    volatile uint32_t tail;
} FIFO_t;

/**
 * @brief Initialize the FIFO
 * @param fifo Pointer to FIFO structure
 * @param buffer Pointer to the data array
 * @param size Size of the array
 */
void fifo_init(FIFO_t *fifo, fifo_data_t *buffer, uint32_t size);

/**
 * @brief Push a value into the FIFO
 * @param fifo Pointer to FIFO structure
 * @param value Value to push
 * @return true if success, false if full
 */
bool fifo_push(FIFO_t *fifo, fifo_data_t value);

/**
 * @brief Pop a value from the FIFO
 * @param fifo Pointer to FIFO structure
 * @param value Pointer to store the popped value
 * @return true if success, false if empty
 */
bool fifo_pop(FIFO_t *fifo, fifo_data_t *value);

/**
 * @brief Check if FIFO is empty
 */
bool fifo_is_empty(FIFO_t *fifo);

/**
 * @brief Check if FIFO is full
 */
bool fifo_is_full(FIFO_t *fifo);

/**
 * @brief Get number of elements in FIFO
 */
uint32_t fifo_count(FIFO_t *fifo);

#endif // FIFO_H
