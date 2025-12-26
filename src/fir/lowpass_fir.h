#ifndef LOWPASS_FIR_H
#define LOWPASS_FIR_H
#include "arm_math.h"


#define FILTER_TAP_NUM 31
#define FILTER_BLOCK_SIZE 32

extern const float32_t filter_kernel[FILTER_TAP_NUM];
extern float32_t filter_state[FILTER_TAP_NUM + FILTER_BLOCK_SIZE - 1];
extern arm_fir_instance_f32 filter;

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

