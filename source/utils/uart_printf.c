/**
 * @file uart_printf.c
 * @addtogroup Level_0_Utilities
 *
 * @brief This file provides a simple printf-like function for UART output.
 *
 * @author domis
 * @date 28.02.2025
 */

// File specific includes
#include "uart_printf.h"

#include "system_settings.h"
#include "uart_hal.h"

// Target specific includes
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

/**
 * @brief Sends a formatted string over UART.
 *
 * @param format A printf-style format string for output.
 * @param ... Additional arguments to format the string.
 */
void UartPrintf_Printf(const char *format, ...)
{
    va_list args;
    int16_t bytes;
    char    msg[UART_PRINTF_BUFFER_SIZE] = {0};

    va_start(args, format);
    bytes = (int16_t)vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);

    if(bytes <= 0) // Error occured
    {
        return;
    }

    // No need for wait for Uart, as this function is blocking itself
    Uart_SendString(msg, (uint8_t)bytes);
}