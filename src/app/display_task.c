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

    while (1)
    {
        // Wait for new data from queue
        if (xQueueReceive(ctx->display_queue, &data, portMAX_DELAY) == pdTRUE) {
            
            for(int i=0; i<DISPLAY_BLOCK_SIZE; i++) {
                char line_buf[80];
                float fft_val = (i < DISPLAY_BLOCK_SIZE/2) ? data.fft_buffer[i] : 0.0f;
                
                int line_len = snprintf(line_buf, sizeof(line_buf), "%lu,%.3f,%.3f,%.2f\r\n", 
                                      data.index_buffer[i], 
                                      data.signal_buffer[i], 
                                      data.filtered_buffer[i],
                                      fft_val);
                
                if (line_len > 0) {
                    // Clamp length if snprintf truncated
                    if (line_len >= (int)sizeof(line_buf)) {
                        line_len = (int)sizeof(line_buf) - 1;
                    }
                    UartDriver_Write((uint8_t*)line_buf, line_len);
                }
            }
        }
    }
}
