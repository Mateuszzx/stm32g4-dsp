#include "driver.h"
#include "arm_math.h"
#include "cmsis_os2.h"
#include <stdint.h>

#define BLOCK_SIZE 5

uint32_t inputA_int[BLOCK_SIZE] = {2043, 1023, 990, 67, 3031};
uint32_t outputA_int[BLOCK_SIZE];
q31_t sensor_data_q31[BLOCK_SIZE];
float32_t inputA[BLOCK_SIZE];
float32_t outputA[BLOCK_SIZE];


void driver_init(void) {
    // Initialization code for the driver
    for(uint8_t i = 0; i < BLOCK_SIZE; i++) {
        inputA[i] = ((float32_t)(inputA_int[i] & 0xFFF)/(0xFFF/2)) - 1;
    }
    arm_float_to_q31(inputA, sensor_data_q31, BLOCK_SIZE);

    // Perform DSP operation: Q31 to Float conversion
    arm_q31_to_float(sensor_data_q31, outputA, BLOCK_SIZE);
    
    float32_t temp;

    for(uint8_t i = 0; i < BLOCK_SIZE; i++) {
        temp = (outputA[i] + 1) * (0xFFF/2);
        outputA_int[i] = (uint32_t)(temp + 0.5f)+1;
        // Round to the nearest integer

    }


}

void driver_loop(void) {
    // Main loop code for the driver
    osDelay(1);
}