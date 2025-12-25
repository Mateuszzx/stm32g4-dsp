#include <stdio.h>
#include <sys/types.h>
#include "FreeRTOS.h"
#include "task.h"
#include "arm_math.h"

#include "driver.h"
#include "sine_generator.h"
#include "uart_driver.h"


#define SIGNAL_FREQUENCY 10
#define SAMPLE_RATE      1000


sine_generator_q15_t sine_gen;

q15_t sample;   

void driver_init(void) {
    UartDriver_Init();
    sine_gen_init_q15(&sine_gen, SAMPLE_RATE, SIGNAL_FREQUENCY);
    

}

void driver_loop(void) {
    sample = sine_calc_sample_q15(&sine_gen);
    printf(">Sample:%d\r\n", (int)sample);
}