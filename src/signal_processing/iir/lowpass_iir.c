/**
 * @file lowpass_iir.c
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Lowpass IIR Filter Implementation
 * @version 0.1
 * @date 2025-12-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "lowpass_iir.h"
#include "iir_filter.h"


/**
 * @brief Coefficients generated using Python iir_coeffs.py script
 * 
 */
#define IIR_NUM_STAGES 3
static const float32_t lowpass_coeffs[15] = { 0.00000018f, 0.00000035f, 0.00000018f, 1.71607129f, -0.73746234f, 1.00000000f, 2.00000000f, 1.00000000f, 1.77863178f, -0.80080265f, 1.00000000f, 2.00000000f, 1.00000000f, 1.89850942f, -0.92217458f };



#define IIR_COEFF_BUFFER_SIZE (5 * IIR_NUM_STAGES) // Each biquad stage has 5 coefficients: b0, b1, b2, -a1, -a2
IIRFilter_t lowpass_filter;
float32_t lowpass_state_buffer[IIR_COEFF_BUFFER_SIZE];


void LowpassIIR_Init(void)
{
    IIRFilter_Init(&lowpass_filter,
                   (const float32_t *)lowpass_coeffs,
                   (float32_t *)lowpass_state_buffer,
                   (uint32_t)IIR_NUM_STAGES);
}

void LowpassIIR_Execute(float32_t *pSrc,
                        float32_t *pDst,
                        uint32_t blockSize)
{
    IIRFilter_Execute(&lowpass_filter,
                      pSrc,
                      pDst,
                      blockSize);
}