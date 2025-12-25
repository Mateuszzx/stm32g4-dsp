#include <stdio.h>
#include <sys/types.h>
#include "FreeRTOS.h"
#include "task.h"
#include "arm_math.h"

#include "driver.h"
#include "sine_generator.h"
#include "uart_driver.h"
#include "lowpass_fir.h"

#define NOISE_FREQUENCY 50
#define SIGNAL_FREQUENCY 10
#define SAMPLE_RATE      1000


sine_generator_q15_t sine_gen;
sine_generator_q15_t noise_gen;

q15_t signal, noise, disturbed_signal, filtered_signal;

void driver_init(void) {
    // Initialization code for the driver
    UartDriver_Init();

    sine_gen_init_q15(&sine_gen, SAMPLE_RATE, SIGNAL_FREQUENCY);
    sine_gen_init_q15(&noise_gen, SAMPLE_RATE, NOISE_FREQUENCY);
    lowpass_init();

}

void driver_loop(void) {
    // Main loop code for the driver
    signal = sine_calc_sample_q15(&sine_gen)/2;
    noise = sine_calc_sample_q15(&noise_gen)/10;
    disturbed_signal = signal + noise;
    
    lowpass_filter_execute(&disturbed_signal, &filtered_signal, 1);
    printf(">Sample:%d,Sample_filt:%d\r\n", (int)disturbed_signal, (int)filtered_signal);

    vTaskDelay(pdMS_TO_TICKS(1));
}