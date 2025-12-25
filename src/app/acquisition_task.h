#ifndef ACQUISITION_TASK_H
#define ACQUISITION_TASK_H
#include "FreeRTOS.h"
#include "semphr.h"
#include "arm_math.h"

#include "driver.h"
#include "sine_generator.h"



#define NOISE_FREQUENCY 50
#define SIGNAL_FREQUENCY 10
#define SAMPLE_RATE      1000


void AcquisitionTask(void *params);

#endif // ACQUISITION_TASK_H
