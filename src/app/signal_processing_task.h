/**
 * @file signal_processing_task.h
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Signal Processing Task Header
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef SIGNAL_PROCESSING_TASK_H
#define SIGNAL_PROCESSING_TASK_H
#include <string.h>
#include "arm_math.h"


#define SINUSOIDAL_TEST_SIGNAL 1
// Signal and Noise parameters
#define SIGNAL_FREQ_HZ 20.0f
#define NOISE_FREQ_HZ 100.0f
#define SAMPLE_RATE_HZ 1000.0f

/**
 * @brief Signal Processing Task
 * 
 * @param params 
 */
void SignalProcessingTask(void *params);

/**
 * @brief Initialize Signal Processing resources
 * 
 */
void SignalProcessing_Init(void);

/**
 * @brief Callback to be called from Acquisition ISR
 * 
 * @param sample Acquired sample
 */
void Acquisition_ISR_Callback(float32_t sample);


/**
 * @brief Notify Signal Processing Task from ISR
 * 
 */
void SignalProcessing_NotifyFromISR(void);

#endif // SIGNAL_PROCESSING_TASK_H