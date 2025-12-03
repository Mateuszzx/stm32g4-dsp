#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "stm32g4xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"

void UartDriver_Init(void);

/**
 * Send data over LPUART1 using DMA.
 * - Safe to call from multiple tasks (internally uses a mutex).
 * - BLOCKS the calling task until TX complete.
 */
int UartDriver_Write(const uint8_t *data, uint16_t len);

#endif // UART_DRIVER_H