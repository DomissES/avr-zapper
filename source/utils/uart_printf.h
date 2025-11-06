/**
 * @file uart_printf.h
 * @addtogroup Level_0_Utilities
 *
 * @brief Header file for UART printf-like functionality.
 *
 * @author domis
 * @date 28.02.2025
 */

#ifndef UART_PRINTF_H_
#define UART_PRINTF_H_

/**
 * @brief Sends a formatted string over UART.
 *
 * @param format A printf-style format string for output.
 * @param ... Additional arguments to format the string.
 */
void UartPrintf_Printf(const char *format, ...);

#endif // UART_PRINTF_H_