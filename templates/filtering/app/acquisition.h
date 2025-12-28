#ifndef ACQUISITION_H
#define ACQUISITION_H

#include "adc.h"

#define DMA_BUFFER_SIZE 256 // 2 blocks of 32 samples each
#define ADC_MAX_VALUE 4095.0f
#define ADC_REF_VOLTAGE 3.3f

// Initialize Acquisition (Start ADC DMA)
void Acquisition_Init(ADC_HandleTypeDef *hadc);

#endif // ACQUISITION_H
