#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <stdint.h>
#include <stdbool.h>


// ADC Configuration Structure
typedef struct {
    uint8_t channel;          // ADC channel number
    uint32_t sampling_rate;   // Sampling rate in Hz
    bool continuous_mode;     // Continuous conversion mode
} ADC_Config;


/**
 * @brief Initialize the ADC with the given configuration
 * 
 */
void ADC_Init(const ADC_Config* config);


/**
 * @brief Start ADC DMA transfer
 * 
 * @param buffer Pointer to the buffer to store ADC samples
 * @param length Number of samples to transfer
 */
void ADC_StartDMA(uint16_t *buffer, uint32_t length);

/**
 * @brief Stop ADC DMA transfer
 * 
 */
void ADC_StopDMA(void);


#endif // ADC_DRIVER_H