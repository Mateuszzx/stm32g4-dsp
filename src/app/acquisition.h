/**
 * @file acquisition.h
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Acquisition Module Header
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ACQUISITION_H
#define ACQUISITION_H

#include "adc.h"

#define DMA_BUFFER_SIZE 512 // 2 blocks of 256 samples each
#define ADC_MAX_VALUE 4095.0f // 12-bit ADC  
#define ADC_REF_VOLTAGE 3.3f  // Reference voltage for ADC

/**
 * @brief Initialize Acquisition (Start ADC DMA)
 * @param hadc Pointer to ADC handle
 */ 
void Acquisition_Init(ADC_HandleTypeDef *hadc);

#endif // ACQUISITION_H
