#include "display_task.h"
#include "driver.h"
#include "uart_driver.h"
#include <stdio.h>
#include <string.h>

// Move large buffers to static memory to prevent stack overflow
static char tx_buf[256];

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
                
                int line_len = snprintf(line_buf, sizeof(line_buf), "%lu,%.4f,%.4f,%.4f\r\n", 
                                      data.index_buffer[i], 
                                      data.signal_buffer[i], 
                                      data.filtered_buffer[i],
                                      fft_val);
                
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
