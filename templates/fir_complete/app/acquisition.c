#include "acquisition.h"
#include "signal_processing_task.h"



static uint16_t adc_dma_buffer[DMA_BUFFER_SIZE];

void Acquisition_Init(ADC_HandleTypeDef *hadc) {
    // Clear Overrun flag just in case
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);

    // Start ADC in DMA Circular mode
    if (HAL_ADC_Start_DMA(hadc, (uint32_t*)adc_dma_buffer, DMA_BUFFER_SIZE) != HAL_OK) {
        // Error handling
        while(1); // Trap for debugging
    }
}

static void Process_DMA_Buffer(uint16_t *buffer, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        // Convert to float (-1.0 to 1.0)
        float32_t sample = ((float32_t)(buffer[i] & 0xFFF) / ADC_MAX_VALUE) * ADC_REF_VOLTAGE;
        Acquisition_ISR_Callback(sample);
    }
    SignalProcessing_NotifyFromISR();
}


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {
        // Process first half
        Process_DMA_Buffer(&adc_dma_buffer[0], DMA_BUFFER_SIZE / 2);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {
        // Process second half
        Process_DMA_Buffer(&adc_dma_buffer[DMA_BUFFER_SIZE / 2], DMA_BUFFER_SIZE / 2);
    }
}
    
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc) {
        // Check for Overrun error
        if (hadc->ErrorCode & HAL_ADC_ERROR_OVR) {
            // Clear the flag and restart? 
            // For now, just trap or log
            __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
            // Ideally, we should restart the ADC here if it stopped
            HAL_ADC_Start_DMA(hadc, (uint32_t*)adc_dma_buffer, DMA_BUFFER_SIZE);
        }
    }
