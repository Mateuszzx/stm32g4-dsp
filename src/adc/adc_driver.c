#include "adc_driver.h"
#include "adc.h"
#include <stddef.h>

// External reference to the ADC handle defined in Core/Src/adc.c
extern ADC_HandleTypeDef hadc1;

void ADC_Init(const ADC_Config* config) {
    // Initialization is handled by CubeMX (MX_ADC1_Init)
    // This function can be used for driver-specific setup if needed in the future
    (void)config; 
}

void ADC_StartDMA(uint16_t *buffer, uint32_t length) {
    // Assumes DMA is configured and linked to ADC1 in CubeMX (HAL_ADC_MspInit)
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)buffer, length) != HAL_OK) {
        Error_Handler();
    }
}

void ADC_StopDMA(void) {
    HAL_ADC_Stop_DMA(&hadc1);
}

/* 
// NOTE: Implement HAL_ADC_ConvCpltCallback in your application (e.g., main.c or acquisition_task.c)
// to handle the conversion complete interrupt.
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {
        // User code here
    }
}
*/

