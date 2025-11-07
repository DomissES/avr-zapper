/**
 * @file uart_hal.h
 * @addtogroup Level_1_HAL
 *
 * @brief Header file for Uart interface
 *
 * In this project Uart interface is used for logging.
 *
 * @author domis
 * @date 29.10.2025
 */

#ifndef UART_HAL_H_
#define UART_HAL_H_

// File specific includes
#include "global_defines.h"

// Target specific includes

//===================================================================================================================//
// Public macro defines                                                                                              //
//===================================================================================================================//

//===================================================================================================================//
// Public definitions                                                                                                //
//===================================================================================================================//

//===================================================================================================================//
// Public variables                                                                                                  //
//===================================================================================================================//

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Initializes the UART hardware.
 *
 * This function configures the UART settings such as baud rate, data bits, stop bits, and parity.
 * It doesn't return any value, nor any input is not required.
 *
 */
void Uart_InitUart();

/**
 * @brief This functions sends a string over UART.
 *
 * The function sends a string in a loop until the specified length is reached.
 * It does it in blocking way, the function doesn't use interrupts or DMA.
 *
 * @param pString pointer to the string to be sent.
 * @param [in] len length of the string to be sent.
 */
void Uart_SendString(const char *cString, uint8_t len);

/**
 * @brief Sends a single byte over UART.
 *
 * This function sends a single byte over UART.
 * It does in a blocking way, the function doesn't use interrupts or DMA.
 *
 * @param [in] c character to be sent
 */
void Uart_SendByte(uint8_t byte);

/**
 * @brief Receives a byte over UART.
 *
 * This functions waits for a byte to be received over UART and returns it.
 * It does in a blocking way, the function doesn't use interrupts or DMA.
 *
 * @return uint8_t character received
 */
uint8_t Uart_ReceiveByte();

#endif // UART_HAL_H_