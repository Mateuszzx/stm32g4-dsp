#include "signal_processing_task.h"
#include "driver.h"
#include "lowpass_fir.h"
#include "fifo.h"
#include "task.h"


#define FIFO_SIZE 128
#define BLOCK_SIZE DSP_BLOCK_SIZE

static float32_t fifo_buffer[FIFO_SIZE];
static float32_t input_block[BLOCK_SIZE];
static float32_t output_block[BLOCK_SIZE];

static FIFO_t adc_fifo;
static TaskHandle_t signalProcessingTaskHandle = NULL;


void SignalProcessing_Init(void) {
    fifo_init(&adc_fifo, fifo_buffer, FIFO_SIZE);
    LowpassFIR_Init();
}


void SignalProcessingTask(void *params)
{
    AppContext *ctx = (AppContext *)params;
    if (ctx == NULL ||  ctx->semaphor == NULL)
    {
        vTaskDelete(NULL);
        return;
    }
    
    signalProcessingTaskHandle = xTaskGetCurrentTaskHandle();
    xSemaphoreGive(ctx->semaphor);

    while (1)
    {
        // Wait for notification from ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Check if we have enough data for a block
        while (fifo_count(&adc_fifo) >= BLOCK_SIZE) {
            // Pop block from FIFO
            for (int i = 0; i < BLOCK_SIZE; i++) {
                fifo_pop(&adc_fifo, &input_block[i]);
            }

            // Process Block
            LowpassFIR_Execute(input_block, output_block, BLOCK_SIZE);

            // Update shared context
            if (xSemaphoreTake(ctx->semaphor, 0) == pdTRUE) {
                memcpy(ctx->signal_buffer, input_block, sizeof(float32_t) * BLOCK_SIZE);
                memcpy(ctx->filtered_buffer, output_block, sizeof(float32_t) * BLOCK_SIZE);
                xSemaphoreGive(ctx->semaphor);
            }
        }
    }
}


// Callback called from Acquisition ISR logic
void Acquisition_ISR_Callback(float32_t sample) {
    // Push to FIFO
    fifo_push(&adc_fifo, sample);
}


void SignalProcessing_NotifyFromISR(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (signalProcessingTaskHandle != NULL) {
        vTaskNotifyGiveFromISR(signalProcessingTaskHandle, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
