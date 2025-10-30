/**
 * @file uart_hal.c
 * @addtogroup Level_1_HAL
 *
 * @brief Source code for first layer of uart interface.
 *
 * In this project uart is used only for logging. It is implemented in blocking mode, with no interrupts used.
 * 
 * @author domis
 * @date 30.10.2025
 */

// File specific includes
#include "uart_hal.h"

#include "global_defines.h"

// Target specific includes
#include <avr/io.h>

//===================================================================================================================//
// Private macro defines                                                                                              //
//===================================================================================================================//



//===================================================================================================================//
// Private definitions                                                                                                //
//===================================================================================================================//



//===================================================================================================================//
// Private variables                                                                                                  //
//===================================================================================================================//


//===================================================================================================================//
// Private functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Sets proper bits in UCSR0A depending on UART_BAUDRATE
 *
 */
static void Uart_privConfigBaud()
{
#define BAUD UART_BAUDRATE
#include "util/setbaud.h"

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif // USE_2X

#undef BAUD
}

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Initializes the UART hardware.
 *
 * This function configures the UART settings such as baud rate, data bits, stop bits, and parity.
 * It doesn't return any value, nor any input is not required.
 */
void Uart_InitUart()
{
    privUart_ConfigBaud();
    // set uart to 8N1
    UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01);
    // enable receive and transmit
    UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
}

/**
 * @brief This functions sends a string over UART.
 *
 * The function sends a string in a loop until the specified length is reached.
 * It does it in blocking way, the function doesn't use interrupts or DMA.
 *
 * @param cString pointer to the string to be sent.
 * @param [in] len length of the string to be sent.
 */
void Uart_SendString(const char *cString, uint8_t len)
{
    while(len--)
    {
        Uart_SendByte((uint8_t)*cString++);
    }
}

/**
 * @brief Sends a single byte over UART.
 *
 * This function sends a single byte over UART.
 * It does in a blocking way, the function doesn't use interrupts or DMA.
 *
 * @param [in] c character to be sent
 */
void Uart_SendByte(uint8_t byte)
{
    while((UCSR0A & _BV(UDRE0)) == 0)
        ;
    UDR0 = byte;
}

/**
 * @brief Receives a byte over UART.
 *
 * This function waits for a byte to be received over UART and returns it.
 * It does in a blocking way, the function doesn't use interrupts or DMA.
 *
 * @return uint8_t character received
 */
uint8_t Uart_ReceiveByte()
{
    while((UCSR0A & _BV(RXC0)) == 0)
        ;
    return UDR0;
}
