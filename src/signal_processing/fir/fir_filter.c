/**
 * @file fir_filter.c
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief FIR filter implementation
 * @version 0.1
 * @date 2025-12-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "fir_filter.h"


void FIR_Init(fir_instance_t *filter, int tap_num, const float32_t *pCoeffs, float32_t *pState, uint32_t block_size)
{
    arm_fir_init_f32(filter,
                tap_num,
                pCoeffs,
                pState, 
                block_size);
}


void FIR_Execute(fir_instance_t *filter, float32_t *input, float32_t *output, uint32_t block_size)
{
    arm_fir_f32(filter, input, output, block_size);
}