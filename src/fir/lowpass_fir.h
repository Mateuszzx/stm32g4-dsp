#ifndef LOWPASS_FIR_H
#define LOWPASS_FIR_H
#include "arm_math.h"


/**
 * @brief Initialize Lowpass FIR filter 
 * 
 */
void LowpassFIR_Init(void);

/**
 * @brief Execute Lowpass FIR filter 
 * 
 * @param input Pointer to the input data buffer
 * @param output Pointer to the output data buffer
 * @param block_size Number of samples to process
 */
void LowpassFIR_Execute(q15_t *input, q15_t *output, uint32_t block_size);

#endif // LOWPASS_FIR_H