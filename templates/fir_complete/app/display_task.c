#include "display_task.h"
#include "driver.h"

void DisplayTask(void *params)
{
    AppContext *ctx = (AppContext *)params;
    if (ctx == NULL ||  ctx->semaphor == NULL)
    {
        vTaskDelete(NULL);
        return;
    }
    
    UartDriver_Init(); // Initialize UART for display output
    
    while (1)
    {
        if (xSemaphoreTake(ctx->semaphor, portMAX_DELAY) == pdTRUE) {
            for(int i=0; i<DSP_BLOCK_SIZE; i++) {
                printf(">Signal: %.4f,Filtered: %.4f\r\n", ctx->signal_buffer[i], ctx->filtered_buffer[i]);
            }
            xSemaphoreGive(ctx->semaphor);
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay for demonstration purposes
    }
}
