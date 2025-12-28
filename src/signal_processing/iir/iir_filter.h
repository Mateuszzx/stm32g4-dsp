/**
 * @file iir_filter.h
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Generic IIR Filter Header File
 * @version 0.1
 * @date 2025-12-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include "arm_math.h"


/**
 * @brief IIR Filter structure
 * 
 */
typedef struct {
    arm_biquad_casd_df1_inst_f32 biquad_instance; /**< ARM CMSIS DSP biquad instance */
    float32_t *pCoeffs;                     /**< Pointer to coefficient buffer */
    float32_t *pState;                            /**< Pointer to state buffer */
    uint32_t numStages;                           /**< Number of biquad stages */
} IIRFilter_t;


/**
 * @brief Initialize the IIR filter with given coefficients
 * 
 * @param filter Pointer to the IIRFilter_t structure
 * @param pCoeffs Pointer to the coefficient array
 * @param pState Pointer to the state buffer
 * @param numStages Number of biquad stages
 */
void IIRFilter_Init(IIRFilter_t *filter,
                    const float32_t *pCoeffs,
                    float32_t *pState,
                    uint32_t numStages);
/**
 * @brief Execute the IIR filter on input data
 * 
 * @param filter Pointer to the IIRFilter_t structure
 * @param pSrc Pointer to the input data array
 * @param pDst Pointer to the output data array
 * @param blockSize Number of samples to process
 */
void IIRFilter_Execute(IIRFilter_t *filter,
                       float32_t *pSrc,
                       float32_t *pDst,
                       uint32_t blockSize);     

#endif // IIR_FILTER_H