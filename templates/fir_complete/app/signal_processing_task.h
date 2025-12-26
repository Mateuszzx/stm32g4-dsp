#ifndef SINAL_PROCESSING_TASK_H
#define SINAL_PROCESSING_TASK_H
#include <string.h>
#include "arm_math.h"


void SignalProcessingTask(void *params);

void SignalProcessing_Init(void);

// Callback to be called from Timer ISR
void Acquisition_ISR_Callback(float32_t sample);

void SignalProcessing_NotifyFromISR(void);

#endif // SINAL_PROCESSING_TASK_H