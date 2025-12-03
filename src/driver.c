#include "driver.h"
#include "arm_math.h"
#include "sine_generator.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#define SIGNAL_FREQUENCY 10
#define SAMPLE_RATE      1000
sine_generator_q15_t sine_gen;
q15_t sample;
float32_t float_sample;


void driver_init(void) {
    // Initialization code for the driver
    sine_generator_q15_t sine;
    sine_gen_init_q15(&sine, SAMPLE_RATE, SIGNAL_FREQUENCY);
    

}

void driver_loop(void) {
    // Main loop code for the driver
    sample = sine_calc_sample_q15(&sine_gen);
    arm_q15_to_float(&sample, &float_sample, 1);
    
    printf("%f", float_sample);
}