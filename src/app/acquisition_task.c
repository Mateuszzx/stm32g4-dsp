#include "acquisition_task.h"




void AcquisitionTask(void *params)
{
    AppContext *ctx = (AppContext *)params;
    if (ctx == NULL ||  ctx->acquisition_sem == NULL)
    {
        vTaskDelete(NULL);
        return;
    } 
    sine_generator_q15_t sine_gen;
    sine_generator_q15_t noise_gen;
    sine_gen_init_q15(&sine_gen, SAMPLE_RATE, SIGNAL_FREQUENCY);
    sine_gen_init_q15(&noise_gen, SAMPLE_RATE, NOISE_FREQUENCY);
    q15_t sample_signal, sample_noise;

    xSemaphoreGive(ctx->acquisition_sem);
    while (1)
    {
        if (xSemaphoreTake(ctx->acquisition_sem, portMAX_DELAY) == pdTRUE) {
            sample_signal = sine_calc_sample_q15(&sine_gen)/2;
            sample_noise = sine_calc_sample_q15(&noise_gen)/10;
            ctx->signal = sample_signal + sample_noise;
        }
        xSemaphoreGive(ctx->acquisition_sem);
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay for demonstration purposes
    }
}