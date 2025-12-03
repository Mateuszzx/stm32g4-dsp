#ifndef SINE_GENERATOR_H
#define SINE_GENERATOR_H

#include "arm_math.h"


typedef struct {
    
    arm_biquad_casd_df1_inst_q15 iir_sine_generator_instance; // Instance structure for the Q15 Biquad cascade filter.
    q15_t coeff[6];
    q15_t state[4];

}sine_generator_q15_t;


void sine_gen_init_q15(sine_generator_q15_t *sine, q15_t sample_rate, q15_t frequency);
q15_t sine_calc_sample_q15(sine_generator_q15_t *sine);

#endif // SINE_GENERATOR_H