#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "stm32g4xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"


/**
 * @brief Initialize the UART driver.
 * 
 */
void UartDriver_Init(void);

/**
* @brief Write data to LPUART1 using DMA.
* 
* @param data Pointer to the data buffer to send.
* @param len Length of the data to send.
* @return int Returns 0 on success, or a negative error code on failure.
*/
int UartDriver_Write(const uint8_t *data, uint16_t len);


#endif // UART_DRIVER_H