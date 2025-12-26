#include <stdio.h>
#include <sys/types.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "arm_math.h"

#include "driver.h"
#include "acquisition.h"
#include "signal_processing_task.h"
#include "display_task.h"
#include "tim.h"
#include "adc.h"


#define NOISE_FREQUENCY 50
#define SIGNAL_FREQUENCY 10
#define SAMPLE_RATE      1000


void Driver_Init(AppContext *ctx) {
    // Initialization code for the driver
    
    if (ctx != NULL) {
        ctx->semaphor = xSemaphoreCreateBinary();
    }
}

void Driver_StartTasks(AppContext *ctx)
{        
    // Initialize Signal Processing resources (FIFO, FIR)
    SignalProcessing_Init();

    xTaskCreate(SignalProcessingTask,
                "SignalProcessingTask",
                TASK_SIGNAL_PROCESSING_STACK_SIZE,
                (void*)ctx,
                TASK_SIGNAL_PROCESSING_PRIORITY,
                NULL);
    
    // Yield to let SignalProcessingTask initialize its handle
    taskYIELD();
                
    xTaskCreate(DisplayTask,
                "DisplayTask",
                TASK_DISPLAY_STACK_SIZE,
                (void*)ctx,
                TASK_DISPLAY_PRIORITY,
                NULL);

    // Initialize Acquisition (Starts ADC DMA)
    Acquisition_Init(&hadc1);
    
    // Start Timer 2 (it triggers ADC via TRGO)
    HAL_TIM_Base_Start(&htim2);
}