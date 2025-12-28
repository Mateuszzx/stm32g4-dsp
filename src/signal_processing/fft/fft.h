/**
 * @file fft.h
 * @brief FFT implementation header file using CMSIS-DSP RFFT
 * 
 * This file contains the definitions and function prototypes for the FFT implementation
 * using the CMSIS-DSP library's Real FFT (RFFT) functions.
 */

#ifndef FFT_H
#define FFT_H

#include "arm_math.h"


#define FFT_SIZE 128

extern float32_t fft_output[2 * FFT_SIZE];
extern float32_t window_buffer[FFT_SIZE];
extern float32_t magnitude_output[FFT_SIZE/2 + 1];

/**
 * @brief FFT Handler structure
 * 
 * Holds the CMSIS-DSP RFFT instance and configuration.
 */
typedef struct {
    arm_rfft_fast_instance_f32 instance; /**< CMSIS-DSP RFFT instance */
    uint16_t fft_size;                   /**< FFT size */
} FFT_Handler;

/**
 * @brief FFT Window structure
 * 
 * Holds the window coefficients.
 */
typedef struct {
    uint16_t size;      /**< Size of the window */
    float32_t* values;  /**< Pointer to the window values array */
} FFT_Window;

/**
 * @brief Initialize FFT handler
 * 
 * Initializes the CMSIS-DSP RFFT instance.
 * 
 * @param handler Pointer to the FFT_Handler structure
 * @param fft_size Size of the FFT (must be a power of 2, e.g., 32, 64, 128, 256, 512, 1024, 2048, 4096)
 * @return ARM_MATH_SUCCESS if initialization is successful, ARM_MATH_ARGUMENT_ERROR otherwise
 */
arm_status fft_init(FFT_Handler* handler, uint16_t fft_size);

/**
 * @brief Compute Real FFT
 * 
 * Computes the RFFT of a real input signal.
 * 
 * @param handler Pointer to the initialized FFT_Handler structure
 * @param input Pointer to the input signal array (real values). Size must be fft_size.
 * @param output Pointer to the output buffer. Size must be fft_size.
 *               The output is packed: {Re[0], Re[N/2], Re[1], Im[1], ..., Re[N/2-1], Im[N/2-1]}
 */
void fft_compute(FFT_Handler* handler, float32_t* input, float32_t* output);

/**
 * @brief Initialize a Hanning window
 * 
 * @param window Pointer to the FFT_Window structure to initialize
 * @param buffer Pointer to the buffer where window values will be stored. Must be of size 'size'.
 * @param size Size of the window
 */
void fft_hanning_window_init(FFT_Window* window, float32_t* buffer, uint16_t size);

/**
 * @brief Apply window to input signal
 * 
 * @param input Pointer to the input signal array
 * @param window Pointer to the initialized FFT_Window structure
 * @param output Pointer to the output array where windowed signal will be stored
 */
void fft_apply_window(float32_t* input, FFT_Window* window, float32_t* output);

/**
 * @brief Compute Magnitude of FFT
 * 
 * Computes the magnitude of the complex FFT output.
 * 
 * @param handler Pointer to the initialized FFT_Handler structure
 * @param fft_output Pointer to the FFT output buffer (packed complex)
 * @param magnitude_output Pointer to the magnitude output buffer. Size must be fft_size/2.
 */
void fft_calculate_magnitude(FFT_Handler* handler, float32_t* fft_output, float32_t* magnitude_output);

#endif // FFT_H