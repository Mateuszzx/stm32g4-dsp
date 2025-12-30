/**
 * @file uart_driver.c
 * @author Mateusz Wójcik (mateuszwojcikv@gmail.com)
 * @brief UART Driver Implementation
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "uart_driver.h"
#include "usart.h"
#include <string.h>
#include <stdbool.h>





static uint8_t uartTxBuf[UART_TX_BUF_SIZE];
static volatile uint16_t txHead = 0;
static volatile uint16_t txTail = 0;
static volatile uint16_t txLen = 0; // Length of current DMA transfer
static volatile bool txBusy = false;

static SemaphoreHandle_t uartTxMutex   = NULL;


void UartDriver_Init(void)
{
    /* Create a mutex to serialize access to UART / printf */
    uartTxMutex = xSemaphoreCreateMutex();
    configASSERT(uartTxMutex != NULL);
    
    txHead = 0;
    txTail = 0;
    txBusy = false;
}

/**
 * @brief Start UART transmission if not already in progress
 * 
 */
static void StartTx(void) {
    if (txHead == txTail) {
        txBusy = false;
        return;
    }

    uint16_t len = 0;
    if (txHead > txTail) {
        len = txHead - txTail;
    } else {
        len = UART_TX_BUF_SIZE - txTail;
    }

    txLen = len;
    txBusy = true;
    HAL_UART_Transmit_DMA(&hlpuart1, &uartTxBuf[txTail], len);
}



int UartDriver_Write(const uint8_t *data, uint16_t len)
{
    if (len == 0 || data == NULL)
        return 0;

    xSemaphoreTake(uartTxMutex, portMAX_DELAY);

    for (uint16_t i = 0; i < len; i++) {
        uint16_t nextHead = (txHead + 1) % UART_TX_BUF_SIZE;
        if (nextHead != txTail) {
            uartTxBuf[txHead] = data[i];
            txHead = nextHead;
        } else {
            // Buffer overflow - drop remaining data or break
            break;
        }
    }

    if (!txBusy) {
        StartTx();
    }

    xSemaphoreGive(uartTxMutex);

    return (int)len;
}


// UART TX Complete Callback
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == LPUART1)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
        // Update tail based on completed transfer
        txTail = (txTail + txLen) % UART_TX_BUF_SIZE;
        
        // Start next transfer if data available
        StartTx();
        
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}



/**
 * @brief printf redirection to UART
 * 
 * @param file File descriptor (unused)
 * @param ptr Pointer to the data to write
 * @param len Length of the data to write
 * @return int Number of bytes written
 */
int _write(int file, char *ptr, int len)
{
    (void)file;  // unused

    if (len <= 0)
        return 0;

    /* Safety: if semaphores not created yet, use blocking HAL */
    if (uartTxMutex == NULL)
    {
        HAL_UART_Transmit(&hlpuart1, (const uint8_t *)ptr, (uint16_t)len, HAL_MAX_DELAY);
        return len;
    }

    /* Safety: if scheduler not running or in ISR, use blocking HAL instead */
    if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING)
    {
        /* Before scheduler starts - use blocking transmit */
        HAL_UART_Transmit(&hlpuart1, (const uint8_t *)ptr, (uint16_t)len, HAL_MAX_DELAY);
        return len;
    }

    return UartDriver_Write((const uint8_t *)ptr, (uint16_t)len);
}

