#include "lowpass_iir.h"
#include "iir_filter.h"


/* 
* Coefficients converted from SOS format (b0, b1, b2, a0, a1, a2) to ARM format (b0, b1, b2, -a1, -a2).
* ARM CMSIS-DSP expects: {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22, ...}
*/
#define IIR_NUM_STAGES 3  
static const float32_t lowpass_coeffs[15] = { 0.00000858f, 0.00001715f, 0.00000858f, 1.46486819f, -0.54025357f, 1.00000000f, 2.00000000f, 1.00000000f, 1.56101808f, -0.64135154f, 1.00000000f, 2.00000000f, 1.00000000f, 1.76124923f, -0.85188703f };

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