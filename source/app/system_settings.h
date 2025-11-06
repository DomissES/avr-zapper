/**
 * @file system_settings.h
 * @addtogroup Level_0_Config
 *
 * @brief This file contains settings which are system related and not user related.
 *
 * @author domis
 * @date 30.10.2025
 */

#ifndef SYSTEM_SETTINGS_H_
#define SYSTEM_SETTINGS_H_

// Uart related
#define UART_BAUDRATE           57600

#define LOG_LEVEL               4
#define LOG_PRINTF_FUNC(...)    UartPrintf_Printf(__VA_ARGS__)
#define UART_PRINTF_BUFFER_SIZE 96
#define LOG_SHOW_FUNCTION_LINES 1

#endif // SYSTEM_SETTINGS_H_