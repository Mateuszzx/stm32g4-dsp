/**
 * @file sine_generator.h
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Sine Wave Generator using IIR Biquad Filter in Q15 format
 * @version 0.1
 * @date 2025-12-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef SINE_GENERATOR_H
#define SINE_GENERATOR_H

#include "arm_math.h"

/**
 * @brief Structure for Q15 Sine Wave Generator using IIR Biquad Filter
 * 
 */
typedef struct {
    
    arm_biquad_casd_df1_inst_q15 iir_sine_generator_instance; // Instance structure for the Q15 Biquad cascade filter.
    q15_t coeff[6];
    q15_t state[4];

}sine_generator_q15_t;


/**
 * @brief Initialize the Q15 Sine Wave Generator
 * 
 * @param sine Pointer to the sine generator structure
 * @param sample_rate Sample rate in Q15 format
 * @param frequency Frequency of the sine wave in Q15 format
 */
void sine_gen_init_q15(sine_generator_q15_t *sine, q15_t sample_rate, q15_t frequency);


/**
 * @brief Calculate a sample of the Q15 Sine Wave
 * 
 * @param sine Pointer to the sine generator structure
 * @return q15_t The calculated sine wave sample in Q15 format
 */
q15_t sine_calc_sample_q15(sine_generator_q15_t *sine);

#endif // SINE_GENERATOR_H