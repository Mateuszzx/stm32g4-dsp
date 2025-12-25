#include "arm_math.h"
#define BLOCK_SIZE 5

uint32_t uint32_in_sensor_data[BLOCK_SIZE] = {2043, 1023, 990, 67, 3031};
q31_t q31_sensor_data[BLOCK_SIZE];
float32_t float32_in_sensor_data[BLOCK_SIZE];
float32_t float32_out_sensor_data[BLOCK_SIZE];
uint32_t uint32_out_sensor_data[BLOCK_SIZE];


void Q_pipeline(void) {
    
    // Convert uint32_t input data to float32_t in range [-1, 1]
    
    for(uint8_t i = 0; i < BLOCK_SIZE; i++) {
        float32_in_sensor_data[i] = ((float32_t)(uint32_in_sensor_data[i] & 0xFFF)/(0xFFF/2)) - 1;
    }

    // Convert float32_t to Q31 format
    arm_float_to_q31(float32_in_sensor_data, q31_sensor_data, BLOCK_SIZE);

    // Perform DSP operation
    // .
    // .
    // .
    // Q31 to Float conversion

    arm_q31_to_float(q31_sensor_data, float32_out_sensor_data, BLOCK_SIZE);
    
    // Convert float32_t output data back to uint32_t format
    for(uint8_t i = 0; i < BLOCK_SIZE; i++) {
        float32_t temp = (float32_out_sensor_data[i] + 1) * (0xFFF/2);
        uint32_out_sensor_data[i] = (uint32_t)(temp + 0.5f); // Round to the nearest integer
    }
}