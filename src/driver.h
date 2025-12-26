#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os2.h"
#include "arm_math.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "acquisition.h"


#define TASK_SIGNAL_PROCESSING_STACK_SIZE    256
#define TASK_DISPLAY_STACK_SIZE              512

#define TASK_SIGNAL_PROCESSING_PRIORITY    osPriorityNormal
#define TASK_DISPLAY_PRIORITY              osPriorityNormal

#define DSP_BLOCK_SIZE 32
#define DISPLAY_BLOCK_SIZE (uint32_t)(DMA_BUFFER_SIZE/2)

/**
 * @brief App context structure to hold shared resources
 * 
 */
typedef struct {
    SemaphoreHandle_t semaphor;
    SemaphoreHandle_t data_ready_sem;
    uint32_t index_buffer[DISPLAY_BLOCK_SIZE];
    float32_t signal_buffer[DISPLAY_BLOCK_SIZE];
    float32_t filtered_buffer[DISPLAY_BLOCK_SIZE];
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