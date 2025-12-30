/**
 * @file sine_generator.c
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Sine Wave Generator using IIR Biquad Filter in Q15 format
 * @version 0.1
 * @date 2025-12-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "sine_generator.h"



void sine_gen_init_q15(sine_generator_q15_t *sine, float32_t sample_rate, float32_t frequency)
{
    // Calculate normalized frequency
    float32_t y[4];
    float32_t coeff4, coeff5;

    y[0] = 0.0f;

    // Calculate sine values normalised by sampling rate for first 3 harmonics
    for (int i = 1; i < 4; i++) {
        y[i] = arm_sin_f32((2.0f * PI * frequency * i) / sample_rate);
    }
    // Calculate coefficients for the biquad filter
    coeff4 = (y[2]/ y[1])/2.0f;
    coeff5 = ((y[1] * y[3] - y[2]*y[2])/(y[1]*y[1]))/2.0f;

    // Convert coefficients to Q15 format
    arm_float_to_q15(&coeff4, &sine->coeff[4], 1);
    arm_float_to_q15(&coeff5, &sine->coeff[5], 1);

    // Iniit biquad filter
    arm_biquad_cascade_df1_init_q15(&(sine->iir_sine_generator_instance), 1, sine->coeff, sine->state, 1);

    // Initialize sate variable
    arm_float_to_q15(&y[1], &(sine->state[3]), 1);
}


q15_t sine_calc_sample_q15(sine_generator_q15_t *sine)
{
    q15_t output;
    q15_t input = 0; 

    // Process the input sample through the biquad filter
    arm_biquad_cascade_df1_q15(&(sine->iir_sine_generator_instance), &input, &output, 1);
    
    return output;
}
