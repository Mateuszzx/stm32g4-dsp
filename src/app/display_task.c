#include "display_task.h"
#include "driver.h"
#include "uart_driver.h"
#include <stdio.h>
#include <string.h>

// Move large buffers to static memory to prevent stack overflow
static float32_t local_signal[DISPLAY_BLOCK_SIZE];
static float32_t local_filtered[DISPLAY_BLOCK_SIZE];
static uint32_t local_index[DISPLAY_BLOCK_SIZE];
static char tx_buf[256];

void DisplayTask(void *params)
{
    AppContext *ctx = (AppContext *)params;
    if (ctx == NULL ||  ctx->semaphor == NULL || ctx->data_ready_sem == NULL)
    {
        vTaskDelete(NULL);
        return;
    }
    
    UartDriver_Init(); // Initialize UART for display output
    
    int tx_len = 0;

    while (1)
    {
        // Wait for new data notification
        if (xSemaphoreTake(ctx->data_ready_sem, portMAX_DELAY) == pdTRUE) {
            
            if (xSemaphoreTake(ctx->semaphor, portMAX_DELAY) == pdTRUE) {
                // Copy data to local buffers to release semaphore quickly
                memcpy(local_signal, ctx->signal_buffer, sizeof(float32_t) * DISPLAY_BLOCK_SIZE);
                memcpy(local_filtered, ctx->filtered_buffer, sizeof(float32_t) * DISPLAY_BLOCK_SIZE);
                memcpy(local_index, ctx->index_buffer, sizeof(uint32_t) * DISPLAY_BLOCK_SIZE);
                xSemaphoreGive(ctx->semaphor);

                tx_len = 0;
                for(int i=0; i<DISPLAY_BLOCK_SIZE; i++) {
                    char line_buf[64];
                    int line_len = snprintf(line_buf, sizeof(line_buf), "%lu,%.4f,%.4f\r\n", local_index[i], local_signal[i], local_filtered[i]);
                    
                    if (line_len <= 0) continue;

                    if (tx_len + line_len >= (int)sizeof(tx_buf)) {
                        UartDriver_Write((uint8_t*)tx_buf, tx_len);
                        tx_len = 0;
                    }
                    
                    memcpy(tx_buf + tx_len, line_buf, line_len);
                    tx_len += line_len;
                }
                
                if (tx_len > 0) {
                    UartDriver_Write((uint8_t*)tx_buf, tx_len);
                }
            }
        }
    }
}
