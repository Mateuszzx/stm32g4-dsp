/**
 * @file lowpass_fir.c
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Lowpass FIR Filter Implementation
 * @version 0.1
 * @date 2025-12-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "lowpass_fir.h"
#include "fir_filter.h"

/*Low pass filter : Fpass 1hz Fstop 3hz Fs 100hz Order 31*/
#define FILTER_TAP_NUM 31
const float32_t filter_kernel[FILTER_TAP_NUM]={
    0.014279,0.016942,0.01968,0.022452,0.025216,0.02793,0.03055,0.033033,0.035336,
    0.037422,0.039253,0.040797,0.042028,0.042923,0.043467,0.043649,0.043467,0.042923,
    0.042028,0.040797,0.039253,0.037422,0.035336,0.033033,0.03055,0.02793,0.025216,
    0.022452,0.01968,0.016942,0.014279
};


#define FILTER_BLOCK_SIZE 32
float32_t filter_state[FILTER_TAP_NUM + FILTER_BLOCK_SIZE - 1];
arm_fir_instance_f32 filter;

void LowpassFIR_Init(void)
{
    FIR_Init(&filter,
                (int)FILTER_TAP_NUM,
                filter_kernel,
                filter_state, 
                (uint32_t)FILTER_BLOCK_SIZE);
}

void LowpassFIR_Execute(float32_t *input, float32_t *output, uint32_t block_size)
{
    FIR_Execute(&filter, input, output, block_size);
}