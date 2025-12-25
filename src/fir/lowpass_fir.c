#include "lowpass_fir.h"

#define FILTER_TAP_NUM 32
#define FILTER_BLOCK_SIZE 1

// Lowpass filter coefficients in Q15 format
q15_t lowpass_filter_coeff[FILTER_TAP_NUM] = 
{
0xFB5C, 0x021C, 0x0219, 0x024E,
0x02AC, 0x0325, 0x03B0, 0x0445,
0x04DC, 0x056E, 0x05F7, 0x0675,
0x06DB, 0x072D, 0x0766, 0x0782,
0x0782, 0x0766, 0x072D, 0x06DB,
0x0675, 0x05F7, 0x056E, 0x04DC,
0x0445, 0x03B0, 0x0325, 0x02AC,
0x024E, 0x0219, 0x021C, 0xFB5C
};


q15_t filter_state[FILTER_TAP_NUM + FILTER_BLOCK_SIZE];
arm_fir_instance_q15 lowpass_filter;


void LowpassFIR_Init(void)
{
    arm_fir_init_q15(&lowpass_filter,
                FILTER_TAP_NUM,
                lowpass_filter_coeff,
                filter_state, 
                FILTER_BLOCK_SIZE);
}

void LowpassFIR_Execute(q15_t *input, q15_t *output, uint32_t block_size)
{
    arm_fir_q15(&lowpass_filter, input, output, block_size);
}