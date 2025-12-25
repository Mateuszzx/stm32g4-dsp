#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os2.h"
#include "arm_math.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define TASK_ACQUISITION_STACK_SIZE          512
#define TASK_SIGNAL_PROCESSING_STACK_SIZE    512
#define TASK_DISPLAY_STACK_SIZE              512

#define TASK_ACQUISITION_PRIORITY          osPriorityNormal
#define TASK_SIGNAL_PROCESSING_PRIORITY    osPriorityNormal
#define TASK_DISPLAY_PRIORITY              osPriorityNormal

/**
 * @brief App context structure to hold shared resources
 * 
 */
typedef struct {
    SemaphoreHandle_t acquisition_sem;
    q15_t signal;
    q15_t filtered_signal;
    // Tutaj możesz dodać inne zasoby współdzielone (kolejki, flagi itp.)
} AppContext;


/**
 * @brief Initialize the driver with the given application context
 * 
 * @param ctx Pointer to the application context
 */
void Driver_Init(AppContext *ctx);


/**
 * @brief Start the driver tasks with the given application context
 * 
 * @param ctx Pointer to the application context
 */
void Driver_StartTasks(AppContext *ctx);

#endif // DRIVER_H