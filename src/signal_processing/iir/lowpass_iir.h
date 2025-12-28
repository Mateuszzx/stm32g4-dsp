/**
 * @file lowpass_iir.h
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Lowpass IIR Filter Interface
 * @version 0.1
 * @date 2025-12-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef LOWPASS_IIR_H
#define LOWPASS_IIR_H
#include "iir_filter.h"



/**
 * @brief Initialize a lowpass IIR filter
 * 
 */
void LowpassIIR_Init(void);  


/**
 * @brief Execute the lowpass IIR filter on input data
 * 
 * @param pSrc Pointer to the input data array
 * @param pDst Pointer to the output data array
 * @param blockSize Number of samples to process
 */
void LowpassIIR_Execute(float32_t *pSrc,
                        float32_t *pDst,
                        uint32_t blockSize);

#endif // LOWPASS_IIR_H