#include "signal_processing_task.h"
#include "driver.h"
#include "lowpass_fir.h"


void SignalProcessingTask(void *params)
{

    AppContext *ctx = (AppContext *)params;
    if (ctx == NULL ||  ctx->acquisition_sem == NULL)
    {
        vTaskDelete(NULL);
        return;
    }
    
    LowpassFIR_Init();
    while (1)
    {
        if (xSemaphoreTake(ctx->acquisition_sem, portMAX_DELAY) == pdTRUE) {
            
            LowpassFIR_Execute(&(ctx->signal), &(ctx->filtered_signal), 1);
        }
        xSemaphoreGive(ctx->acquisition_sem);
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay for demonstration purposes
    }
}