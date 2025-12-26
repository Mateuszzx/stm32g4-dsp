#include "acquisition_task.h"
#include "adc_driver.h"
#include "adc.h"

#define ADC_BUFFER_SIZE 1

static uint16_t adc_buffer[ADC_BUFFER_SIZE];
static AppContext *global_ctx = NULL;

// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
//     if (hadc->Instance == ADC1) {
//         BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//         if (global_ctx != NULL && global_ctx->acquisition_sem != NULL) {
//             xSemaphoreGiveFromISR(global_ctx->acquisition_sem, &xHigherPriorityTaskWoken);
//             portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//         }
//     }
// }

void AcquisitionTask(void *params)
{
    AppContext *ctx = (AppContext *)params;
    if (ctx == NULL ||  ctx->acquisition_sem == NULL)
    {
        vTaskDelete(NULL);
        return;
    } 

    /* Sine Generator Code (Commented Out)
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
    */

    global_ctx = ctx;
    float32_t float32_in_sensor_data;

    xSemaphoreGive(ctx->acquisition_sem);

    while (1)
    {
        // Start ADC conversion via DMA
        ADC_StartDMA(adc_buffer, ADC_BUFFER_SIZE);

        // Wait for conversion complete
        if (xSemaphoreTake(ctx->acquisition_sem, portMAX_DELAY) == pdTRUE) {

            float32_in_sensor_data = ((float32_t)(adc_buffer[0] & 0xFFF)/(0xFFF/2)) - 1;
            arm_float_to_q15(&float32_in_sensor_data, &(ctx->signal), 1);

        }
        xSemaphoreGive(ctx->acquisition_sem);
        
        vTaskDelay(pdMS_TO_TICKS(10)); // 100Hz sampling rate
    }
}