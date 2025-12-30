/**
 * @file display_task.c
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief Display Task Implementation
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "display_task.h"
#include "driver.h"
#include "uart_driver.h"
#include <stdio.h>
#include <string.h>

// Move large buffers to static memory to prevent stack overflow
static char tx_buf[UART_TX_BUF_SIZE]; // 4KB buffer for UART transmission chunking


void DisplayTask(void *params)
{
    AppContext *ctx = (AppContext *)params;
    if (ctx == NULL ||  ctx->display_queue == NULL)
    {
        vTaskDelete(NULL);
        return;
    }
    
    UartDriver_Init(); // Initialize UART for display output
    
    static DisplayData_t data;
    int tx_len = 0;

    while (1)
    {
        // Wait for new data from queue
        if (xQueueReceive(ctx->display_queue, &data, portMAX_DELAY) == pdTRUE) {
            
            tx_len = 0;
            for(int i=0; i<DISPLAY_BLOCK_SIZE; i++) {
                char line_buf[80];
                float fft_val = (i < DISPLAY_BLOCK_SIZE/2) ? data.fft_buffer[i] : 0.0f;
                
                int line_len = snprintf(line_buf, sizeof(line_buf), "%lu,%.4f,%.4f,%.2f\r\n", 
                                      data.index_buffer[i], 
                                      data.signal_buffer[i], 
                                      data.filtered_buffer[i],
                                      fft_val);
                
                if (line_len <= 0) continue;
                
                // Safety clamp to prevent buffer read overflow if snprintf returns > sizeof(line_buf)
                if (line_len > (int)sizeof(line_buf)) {
                    line_len = (int)sizeof(line_buf);
                }

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
