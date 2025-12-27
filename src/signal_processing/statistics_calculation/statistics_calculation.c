#include "statistics_calculation.h"


float32_t CalculateMean(const float32_t *data, uint32_t length)
{
    float32_t sum = 0.0f;
    for (uint32_t i = 0; i < length; i++)
    {
        sum += data[i];
    }
    return sum / (float32_t)length;
}

float32_t CalculateVariance(const float32_t *data, uint32_t length, float32_t mean)
{
    float32_t sumSqDiff = 0.0f;
    for (uint32_t i = 0; i < length; i++)
    {
        float32_t diff = data[i] - mean;
        sumSqDiff += diff * diff;
    }
    return sumSqDiff / (float32_t)length;
}

float32_t CalculateStandardDeviation(const float32_t *data, uint32_t length, float32_t mean)
{
    float32_t variance = CalculateVariance(data, length, mean);
    float32_t stdDev;
    arm_sqrt_f32(variance, &stdDev);
    return stdDev;
}

void CalculateStatistics(const float32_t *data, uint32_t length, Statistics_t *stats)
{
    stats->mean = CalculateMean(data, length);
    stats->variance = CalculateVariance(data, length, stats->mean);
    arm_sqrt_f32(stats->variance, &(stats->stdDev));
}

void CalculateRunningMean(const float32_t data, uint32_t sample_number, float32_t *runningMean)
{
    *runningMean += (data - *runningMean) / (float32_t)sample_number;
}

void CalculateRunningVariance(const float32_t data, uint32_t sample_number, const float32_t *previousMean, float32_t *runningVariance)
{
    float32_t actualMean = *previousMean + (data - *previousMean) / (float32_t)sample_number;
    float32_t diff_new = data - *previousMean;
    float32_t diff_old = data - actualMean;
    *runningVariance += (diff_new * diff_old) / (float32_t)sample_number;
}

void CalculateRunningStatistics(const float32_t data, uint32_t sample_number, Statistics_t *runningStats)
{
    float32_t previousMean = runningStats->mean;
    CalculateRunningMean(data, sample_number, &runningStats->mean);
    CalculateRunningVariance(data, sample_number, &previousMean, &runningStats->variance);
    arm_sqrt_f32(runningStats->variance, &(runningStats->stdDev));
}