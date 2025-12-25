#include <stdio.h>
#include <sys/types.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "arm_math.h"

#include "driver.h"
#include "acquisition_task.h"
#include "signal_processing_task.h"
#include "display_task.h"


#define NOISE_FREQUENCY 50
#define SIGNAL_FREQUENCY 10
#define SAMPLE_RATE      1000


void Driver_Init(AppContext *ctx) {
    // Initialization code for the driver
    
    if (ctx != NULL) {
        ctx->acquisition_sem = xSemaphoreCreateBinary();
    }
}

void Driver_StartTasks(AppContext *ctx)
{
    // Start acqusition task
    xTaskCreate(AcquisitionTask,
                "AcquisitionTask",
                TASK_ACQUISITION_STACK_SIZE,
                (void*)ctx,
                TASK_ACQUISITION_PRIORITY,
                NULL);
        
    xTaskCreate(SignalProcessingTask,
                "SignalProcessingTask",
                TASK_SIGNAL_PROCESSING_STACK_SIZE,
                (void*)ctx,
                TASK_SIGNAL_PROCESSING_PRIORITY,
                NULL);
                
    xTaskCreate(DisplayTask,
                "DisplayTask",
                TASK_DISPLAY_STACK_SIZE,
                (void*)ctx,
                TASK_DISPLAY_PRIORITY,
                NULL);
                

}