#include "driver.h"
#include "arm_math.h"
#include "sine_generator.h"
#include "uart_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <sys/types.h>

#define SIGNAL_FREQUENCY 10
#define SAMPLE_RATE      1000
sine_generator_q15_t sine_gen;
q15_t sample;
float32_t float_sample;
uint32_t sample_rate = SAMPLE_RATE;

void driver_init(void) {
    // Initialization code for the driver
    UartDriver_Init();
    sine_gen_init_q15(&sine_gen, SAMPLE_RATE, SIGNAL_FREQUENCY);
    

}

void driver_loop(void) {
    // Main loop code for the driver
    sample = sine_calc_sample_q15(&sine_gen);
    
    printf(">Sample:%d\r\n", (int)sample);

}