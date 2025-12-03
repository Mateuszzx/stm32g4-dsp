#include "uart_driver.h"
#include "usart.h"      // for extern UART_HandleTypeDef hlpuart1
#include <string.h>

/* ---------- configuration ---------- */
#define UART_TX_BUF_SIZE 256   // max single printf line / chunk

/* ---------- static data ---------- */
static uint8_t uartTxBuf[UART_TX_BUF_SIZE];

static SemaphoreHandle_t uartTxMutex   = NULL;
static SemaphoreHandle_t uartTxDoneSem = NULL;

/* ---------- init ---------- */

void UartDriver_Init(void)
{
    /* Create a mutex to serialize access to UART / printf */
    uartTxMutex = xSemaphoreCreateMutex();
    configASSERT(uartTxMutex != NULL);

    /* Binary semaphore given from DMA complete ISR */
    uartTxDoneSem = xSemaphoreCreateBinary();
    configASSERT(uartTxDoneSem != NULL);
}

/* ---------- main write function (for tasks) ---------- */

int UartDriver_Write(const uint8_t *data, uint16_t len)
{
    if (len == 0 || data == NULL)
        return 0;

    if (len > UART_TX_BUF_SIZE)
        len = UART_TX_BUF_SIZE;     // simple clamp; you can improve later

    /* Make sure we don't interleave prints from different tasks */
    xSemaphoreTake(uartTxMutex, portMAX_DELAY);

    memcpy(uartTxBuf, data, len);

    /* Start DMA transfer on LPUART1 */
    HAL_StatusTypeDef st = HAL_UART_Transmit_DMA(&hlpuart1, uartTxBuf, len);
    if (st != HAL_OK)
    {
        xSemaphoreGive(uartTxMutex);
        return 0;
    }

    /* Wait until DMA transfer is done (signaled from ISR) */
    xSemaphoreTake(uartTxDoneSem, portMAX_DELAY);

    xSemaphoreGive(uartTxMutex);

    return (int)len;
}

/* ---------- HAL callback (from DMA interrupt) ---------- */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == LPUART1)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(uartTxDoneSem, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}


/* ---------- printf redirection ---------- */
/* newlib / newlib-nano calls this for printf, puts, etc. */


int _write(int file, char *ptr, int len)
{
    (void)file;  // unused

    if (len <= 0)
        return 0;

    /* Safety: if scheduler not running or in ISR, use blocking HAL instead */
    if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING)
    {
        /* Before scheduler starts - use blocking transmit */
        HAL_UART_Transmit(&hlpuart1, (const uint8_t *)ptr, (uint16_t)len, HAL_MAX_DELAY);
        return len;
    }

    return UartDriver_Write((const uint8_t *)ptr, (uint16_t)len);
}

