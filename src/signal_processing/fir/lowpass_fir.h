/**
 * @file lowpass_fir.h
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Lowpass FIR Filter Header File
 * @version 0.1
 * @date 2025-12-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef LOWPASS_FIR_H
#define LOWPASS_FIR_H
#include "arm_math.h"


/**
 * @brief Initialize FIR filter 
 * 
 */
void LowpassFIR_Init(void);

/**
 * @brief Execute FIR filter 
 * 
 * @param input Pointer to the input data buffer
 * @param output Pointer to the output data buffer
 * @param block_size Number of samples to process
 */
void LowpassFIR_Execute(float32_t *input, float32_t *output, uint32_t block_size);

#endif // LOWPASS_FIR_H

