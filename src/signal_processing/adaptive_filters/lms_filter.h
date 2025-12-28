/**
 * @file lms_filter.h
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief LMS Adaptive Filter Header
 * @version 0.1
 * @date 2025-12-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef LMS_FILTER_H
#define LMS_FILTER_H

#include <stdint.h>
#include "arm_math.h"


typedef struct {
    arm_lms_instance_f32 lms_instance; /**< ARM CMSIS DSP LMS instance */
    float32_t *pCoeffs;                /**< Pointer to filter coefficients */
    float32_t *pState;                 /**< Pointer to state buffer */
    uint32_t filterOrder;              /**< Order of the filter */
    float32_t mu;                      /**< Step size for adaptation */
} LMSFilter_t;


/**
 * @brief Initialize the LMS adaptive filter
 * 
 * @param filter Pointer to the LMSFilter_t structure
 * @param filterOrder Order of the filter
 * @param mu Step size for adaptation
 * @param blockSize Number of samples to process
 */
void LMSFilter_Init(LMSFilter_t *filter, uint32_t filterOrder, float32_t mu, uint32_t blockSize);

/**
 * @brief Execute the LMS adaptive filter
 * 
 * @param filter Pointer to the LMSFilter_t structure
 * @param pInput Pointer to the input signal array
 * @param pDesired Pointer to the desired signal array
 * @param pOutput Pointer to the output signal array
 * @param error Pointer to the error signal array
 * @param blockSize Number of samples to process
 */
void LMSFilter_Execute(LMSFilter_t *filter,
                       float32_t *pInput,
                       float32_t *pDesired,
                       float32_t *pOutput,
                       float32_t *error,
                       uint32_t blockSize);

#endif // LMS_FILTER_H