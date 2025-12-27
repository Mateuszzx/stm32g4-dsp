#ifndef NOSISE_CANCELATION_H
#define NOSISE_CANCELATION_H

#include <stdint.h>
#include "arm_math.h"


/**
 * @brief Initialize the noise cancelation adaptive filter
 * 
 */
void NoiseCancelation_Init(void);

/**
 * @brief Execute the noise cancelation adaptive filter
 * 
 * @param pInput Pointer to the primary input signal array
 * @param pReference Pointer to the desired signal array
 * @param pOutput Pointer to the output (cleaned) signal array
 * @param blockSize Number of samples to process
 */
void NoiseCancelation_Execute(float32_t *pInput,
                              float32_t *pReference,
                              float32_t *pOutput,
                              uint32_t blockSize);


#endif // NOSISE_CANCELATION_H
