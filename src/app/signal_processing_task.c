/**
 * @file signal_processing_task.c
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief  Signal Processing Task Implementation
 * @version 0.2
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "signal_processing_task.h"
#include "driver.h"
#include "lowpass_iir.h"
#include "fifo.h"
#include "task.h"
#include "fft.h"
#include "arm_math.h"
#include "sine_generator.h"

// Signal and Noise parameters
#define SIGNAL_FREQ_HZ 20.0f
#define SAMPLE_RATE_HZ 1000.0f
#define NOISE_FREQ_HZ 70.0f

static sine_generator_q15_t signal_gen;
static sine_generator_q15_t noise_gen;

static q15_t signal, noise, combined;
static float32_t sample_combined;
////////////////////////////////////

// FIFO parameters
#define FIFO_SIZE 256
#define BLOCK_SIZE DSP_BLOCK_SIZE

static float32_t fifo_buffer[FIFO_SIZE];
static FIFO_t adc_fifo;
////////////////////////////////////


// Accumulation buffer for display
static DisplayData_t display_data;
static uint32_t acc_count = 0;
static uint32_t index_counter = 0;

static TaskHandle_t signalProcessingTaskHandle = NULL;


// FFT related variables
static FFT_Handler fft_handler;
static FFT_Window fft_window;
static float32_t fft_window_buffer[DISPLAY_BLOCK_SIZE];
static float32_t fft_input_buffer[DISPLAY_BLOCK_SIZE];
static float32_t fft_output_buffer[DISPLAY_BLOCK_SIZE];


void SignalProcessing_Init(void)
{
    // Initialize FIFO
    fifo_init(&adc_fifo, fifo_buffer, FIFO_SIZE);
    
    // Initialize Sine Generators
    sine_gen_init_q15(&signal_gen, SAMPLE_RATE_HZ, SIGNAL_FREQ_HZ );
    sine_gen_init_q15(&noise_gen, SAMPLE_RATE_HZ, NOISE_FREQ_HZ);

    // Initialize Lowpass IIR Filter
    LowpassIIR_Init();

    // Initialize FFT
    fft_init(&fft_handler, DISPLAY_BLOCK_SIZE);
    fft_hanning_window_init(&fft_window, fft_window_buffer, DISPLAY_BLOCK_SIZE);

    acc_count = 0;
}

/**
 * @brief Process a single block of data from FIFO
 * 
 */
static void ProcessBlock(void)
{
    // Safety check to prevent buffer overflow
    if (acc_count + BLOCK_SIZE > DISPLAY_BLOCK_SIZE)
    {
        acc_count = 0;
    }

    // Pop block from FIFO directly to accumulation buffer
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        fifo_pop(&adc_fifo, &(display_data.signal_buffer[acc_count + i]));
        display_data.index_buffer[acc_count + i] = index_counter++;
    }

    /* IIR */
    LowpassIIR_Execute(&display_data.signal_buffer[acc_count], &display_data.filtered_buffer[acc_count], BLOCK_SIZE);
    
    // Update accumulation count
    acc_count += BLOCK_SIZE;
}

/**
 * @brief Process FFT on the accumulated data
 * 
 */
static void ProcessFFT(void) {
    // 1. Apply Window
    fft_apply_window(display_data.signal_buffer, &fft_window, fft_input_buffer);
    
    // 2. Compute FFT
    fft_compute(&fft_handler, fft_input_buffer, fft_output_buffer);
    
    // 3. Calculate Magnitude
    fft_calculate_magnitude(&fft_handler, fft_output_buffer, display_data.fft_buffer);
}

/**
 * @brief Send accumulated data to display queue
 * 
 * @param ctx Pointer to application context
 */
static void SendToDisplay(AppContext *ctx) {
     // Send to queue based on mode
    #if DISPLAY_QUEUE_BLOCKING_MODE
        // Blocking: Wait until space is available (guarantees data delivery)
        xQueueSend(ctx->display_queue, &display_data, portMAX_DELAY);
    #else
        // Mailbox: Overwrite with newest data (guarantees latest data, may skip)
        xQueueOverwrite(ctx->display_queue, &display_data);
    #endif
    
    acc_count = 0; // Reset accumulation
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
        while (fifo_count(&adc_fifo) >= BLOCK_SIZE)
        {    
            ProcessBlock();

            // If accumulation buffer is full, send to queue
            if (acc_count >= DISPLAY_BLOCK_SIZE)
            {    
                ProcessFFT();
                SendToDisplay(ctx);
            }
        }
    }
}

// Callback called from Acquisition ISR logic
void Acquisition_ISR_Callback(float32_t sample)
{
    // Generate synthetic signal + noise
    signal = sine_calc_sample_q15(&signal_gen)/2;
    noise = sine_calc_sample_q15(&noise_gen)/2;
    combined = signal+noise;
    arm_q15_to_float(&combined, &sample_combined, 1);
    
    // Push to FIFO
    fifo_push(&adc_fifo, sample_combined);
}

void SignalProcessing_NotifyFromISR(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (signalProcessingTaskHandle != NULL) {
        vTaskNotifyGiveFromISR(signalProcessingTaskHandle, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
