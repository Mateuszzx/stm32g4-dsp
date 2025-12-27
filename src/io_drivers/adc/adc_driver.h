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

void ADC_Init(const ADC_Config* config);

uint16_t ADC_Read(void);
void ADC_StartDMA(uint16_t *buffer, uint32_t length);
void ADC_StopDMA(void);
void ADC_Deinit(void);


#endif // ADC_DRIVER_H