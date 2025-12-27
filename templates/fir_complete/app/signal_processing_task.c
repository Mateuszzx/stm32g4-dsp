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
static uint32_t index_block[BLOCK_SIZE];

// Accumulation buffers for display
static float32_t acc_signal[DISPLAY_BLOCK_SIZE];
static float32_t acc_filtered[DISPLAY_BLOCK_SIZE];
static uint32_t acc_index[DISPLAY_BLOCK_SIZE];
static uint32_t acc_count = 0;

static FIFO_t adc_fifo;
static TaskHandle_t signalProcessingTaskHandle = NULL;
static uint32_t index_counter = 0;

void SignalProcessing_Init(void) {
    fifo_init(&adc_fifo, fifo_buffer, FIFO_SIZE);
    LowpassFIR_Init();
    acc_count = 0;
}


void SignalProcessingTask(void *params)
{
    AppContext *ctx = (AppContext *)params;
    if (ctx == NULL ||  ctx->display_queue == NULL)
    {
        vTaskDelete(NULL);
        return;
    }
    
    signalProcessingTaskHandle = xTaskGetCurrentTaskHandle();

    while (1)
    {
        // Wait for notification from ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Check if we have enough data for a block
        while (fifo_count(&adc_fifo) >= BLOCK_SIZE) {
            // Pop block from FIFO
            for (int i = 0; i < BLOCK_SIZE; i++) {
                fifo_pop(&adc_fifo, &input_block[i]);
                index_block[i] = index_counter++;
            }

            // Process Block
            LowpassFIR_Execute(input_block, output_block, BLOCK_SIZE);

            // Accumulate for display
            if (acc_count + BLOCK_SIZE <= DISPLAY_BLOCK_SIZE) {
                memcpy(&acc_signal[acc_count], input_block, sizeof(float32_t) * BLOCK_SIZE);
                memcpy(&acc_filtered[acc_count], output_block, sizeof(float32_t) * BLOCK_SIZE);
                memcpy(&acc_index[acc_count], index_block, sizeof(uint32_t) * BLOCK_SIZE);
                acc_count += BLOCK_SIZE;
            }

            // If accumulation buffer is full, send to queue
            if (acc_count >= DISPLAY_BLOCK_SIZE) {
                DisplayData_t data;
                memcpy(data.signal_buffer, acc_signal, sizeof(float32_t) * DISPLAY_BLOCK_SIZE);
                memcpy(data.filtered_buffer, acc_filtered, sizeof(float32_t) * DISPLAY_BLOCK_SIZE);
                memcpy(data.index_buffer, acc_index, sizeof(uint32_t) * DISPLAY_BLOCK_SIZE);
                
                // Send to queue based on mode
                #if DISPLAY_QUEUE_BLOCKING_MODE
                    // Blocking: Wait until space is available (guarantees data delivery)
                    xQueueSend(ctx->display_queue, &data, portMAX_DELAY);
                #else
                    // Mailbox: Overwrite with newest data (guarantees latest data, may skip)
                    xQueueOverwrite(ctx->display_queue, &data);
                #endif
                
                acc_count = 0; // Reset accumulation
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
