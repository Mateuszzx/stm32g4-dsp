#ifndef ACQUISITION_H
#define ACQUISITION_H

#include "adc.h"

// Initialize Acquisition (Start ADC DMA)
void Acquisition_Init(ADC_HandleTypeDef *hadc);

#endif // ACQUISITION_H
