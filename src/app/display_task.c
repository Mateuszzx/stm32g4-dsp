#include "display_task.h"
#include "driver.h"

void DisplayTask(void *params)
{
    AppContext *ctx = (AppContext *)params;
    if (ctx == NULL ||  ctx->acquisition_sem == NULL)
    {
        vTaskDelete(NULL);
        return;
    }
    
    UartDriver_Init(); // Initialize UART for display output
    
    while (1)
    {
        if (xSemaphoreTake(ctx->acquisition_sem, portMAX_DELAY) == pdTRUE) {
            printf(">Signal: %d,Filtered: %d\r\n", ctx->signal, ctx->filtered_signal);
        }
        xSemaphoreGive(ctx->acquisition_sem);
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay for demonstration purposes
    }
}
