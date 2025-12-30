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
#define FILTER_TAP_NUM 51
static const float fir_coeffs[51] = { -0.00000000f, -0.00027475f, -0.00062725f, -0.00110792f, -0.00174513f, -0.00253069f, -0.00340869f, -0.00426993f, -0.00495324f, -0.00525452f, -0.00494294f, -0.00378331f, -0.00156235f, 0.00188384f, 0.00664428f, 0.01271256f, 0.01997412f, 0.02820325f, 0.03707091f, 0.04616324f, 0.05500945f, 0.06311682f, 0.07000955f, 0.07526770f, 0.07856257f, 0.07968482f, 0.07856257f, 0.07526770f, 0.07000955f, 0.06311682f, 0.05500945f, 0.04616324f, 0.03707091f, 0.02820325f, 0.01997412f, 0.01271256f, 0.00664428f, 0.00188384f, -0.00156235f, -0.00378331f, -0.00494294f, -0.00525452f, -0.00495324f, -0.00426993f, -0.00340869f, -0.00253069f, -0.00174513f, -0.00110792f, -0.00062725f, -0.00027475f, -0.00000000f };


#define FILTER_BLOCK_SIZE 32
float32_t filter_state[FILTER_TAP_NUM + FILTER_BLOCK_SIZE - 1];
arm_fir_instance_f32 filter;

void LowpassFIR_Init(void)
{
    FIR_Init(&filter,
                (int)FILTER_TAP_NUM,
                fir_coeffs,
                filter_state, 
                (uint32_t)FILTER_BLOCK_SIZE);
}

void LowpassFIR_Execute(float32_t *input, float32_t *output, uint32_t block_size)
{
    FIR_Execute(&filter, input, output, block_size);
}