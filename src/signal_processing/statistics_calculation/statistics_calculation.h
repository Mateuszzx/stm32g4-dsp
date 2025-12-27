#ifndef STATISTICS_CALCULATION_H
#define STATISTICS_CALCULATION_H

#include <stdint.h>
#include "arm_math.h"


typedef struct {
    float32_t mean;
    float32_t variance;
    float32_t stdDev;
} Statistics_t;


/**
 * @brief Calculate the mean of a dataset
 * 
 * @param data Pointer to the data array
 * @param length Number of elements in the data array
 * @return Mean of the data
 */
float32_t CalculateMean(const float32_t *data, uint32_t length);
    
/**
 * @brief Calculate the variance of a dataset
 * 
 * @param data Pointer to the data array
 * @param length Number of elements in the data array
 * @param mean Pre-calculated mean of the data
 * @return Variance of the data
 */
float32_t CalculateVariance(const float32_t *data, uint32_t length, float32_t mean);

/**
 * @brief Calculate the standard deviation of a dataset
 * 
 * @param data Pointer to the data array
 * @param length Number of elements in the data array
 * @param mean Pre-calculated mean of the data
 * @return Standard deviation of the data
 */
float32_t CalculateStandardDeviation(const float32_t *data, uint32_t length, float32_t mean);

/**
 * @brief Calculate basic statistics (mean, variance, stdDev) for a dataset
 * 
 * @param data Pointer to the data array
 * @param length Number of elements in the data array
 * @param stats Pointer to Statistics_t structure to hold the results
 */
void CalculateStatistics(const float32_t *data, uint32_t length, Statistics_t *stats);


// Running statistics calculations

/**
 * @brief CalculateRunningMean using Welford's method
 * 
 * @param data New data sample
 * @param sample_number Current sample number (1-based)
 * @param runningMean Pointer to the running mean value
 */
void CalculateRunningMean(const float32_t data, uint32_t sample_number, float32_t *runningMean);


/**
 * @brief  CalculateRunningVariance. Welford's method for numerical stability
 * 
 * @param data New data sample
 * @param sample_number Current sample number (1-based)
 * @param runningMean Pointer to the running mean value
 * @param runningVariance Pointer to the running variance value
 */
void CalculateRunningVariance(const float32_t data, uint32_t sample_number, const float32_t *runningMean, float32_t *runningVariance);

/**
 * @brief Calculate running statistics (mean, variance, stdDev) for streaming data
 * 
 * @param data New data sample
 * @param length Current sample number (1-based)
 * @param stats Pointer to Statistics_t structure to hold running statistics
 */
void CalculateRunningStatistics(const float32_t data, uint32_t sample_number, Statistics_t *stats);


#endif // STATISTICS_CALCULATION_H