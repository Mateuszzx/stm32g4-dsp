#ifndef FIR_FILTER_H
#define FIR_FILTER_H
#include "arm_math.h"


typedef arm_fir_instance_f32 fir_instance_t;

/**
 * @brief Initialize FIR filter 
 * 
 * @param filter Pointer to the FIR filter instance
 * @param tap_num Number of filter taps
 * @param pCoeffs Pointer to the filter coefficients
 * @param pState Pointer to the filter state buffer
 * @param block_size Number of samples to process
 */
void FIR_Init(fir_instance_t *filter, int tap_num, const float32_t *pCoeffs, float32_t *pState, uint32_t block_size);

/**
 * @brief Execute FIR filter 
 * 
 * @param filter Pointer to the FIR filter instance
 * @param input Pointer to the input data buffer
 * @param output Pointer to the output data buffer
 * @param block_size Number of samples to process
 */
void FIR_Execute(fir_instance_t *filter, float32_t *input, float32_t *output, uint32_t block_size);

#endif // FIR_FILTER_H
