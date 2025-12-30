/**
 * @file fft.c
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief FFT implementation using CMSIS-DSP RFFT
 * @version 0.1
 * @date 2025-12-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "fft.h"
#include <math.h>


arm_status fft_init(FFT_Handler* handler, uint16_t fft_size) {
    handler->fft_size = fft_size;
    return arm_rfft_fast_init_f32(&handler->instance, fft_size);
}

void fft_compute(FFT_Handler* handler, float32_t* input, float32_t* output) {
    // 0 flag for forward FFT
    arm_rfft_fast_f32(&handler->instance, input, output, 0);
}

void fft_hanning_window_init(FFT_Window* window, float32_t* buffer, uint16_t size) {
    window->size = size;
    window->values = buffer;
    
    for (uint16_t i = 0; i < size; i++) {
        // Hanning window formula: 0.5 * (1 - cos(2*pi*n / (N-1)))
        // Using arm_cos_f32 for consistency, though standard cosf works too.
        float32_t angle = 2.0f * PI * (float32_t)i / (float32_t)(size - 1);
        window->values[i] = 0.5f * (1.0f - arm_cos_f32(angle));
    }
}

void fft_apply_window(float32_t* input, FFT_Window* window, float32_t* output) {
    // Element-wise multiplication
    arm_mult_f32(input, window->values, output, window->size);
}

void fft_calculate_magnitude(FFT_Handler* handler, float32_t* fft_output, float32_t* magnitude_output) {
    // fft_output format from arm_rfft_fast_f32:
    // {Re[0], Re[N/2], Re[1], Im[1], Re[2], Im[2], ..., Re[N/2-1], Im[N/2-1]}
    
    // DC component (Bin 0)
    // Re[0] is at index 0
    magnitude_output[0] = fabsf(fft_output[0]);

    // Calculate magnitude for bins 1 to N/2 - 1
    // These are stored as complex pairs starting at index 2
    // Number of such bins is (N/2) - 1
    if (handler->fft_size > 2) {
        arm_cmplx_mag_f32(&fft_output[2], &magnitude_output[1], (handler->fft_size / 2) - 1);
    }
}
