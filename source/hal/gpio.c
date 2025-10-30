/**
 * @file gpio.c
 * @addtogroup Level_1_HAL
 * @brief Source file for GPIO (General Purpose Input/Output) HAL (Hardware Abstraction Layer).
 *
 * This file provides the interface for GPIO operations, including initialization,
 * configuration, and control of GPIO pins.
 *
 * @author domis
 * @date 25.09.2025
 *
 */

// File specific include
#include "gpio.h"

// Target specific includes
#include <avr/io.h>

//===================================================================================================================//
// Public Functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Initializes all GPIO pins and sets their initial states.
 *
 *  This function is specific to current hardware configuration. Refer to the documentation for details.
 *
 * @param[in]  None
 * @param[out] None
 */
void Gpio_InitAll()
{
    // PORT B
    // Direction:   | OUT | OUT | OUT | OUT || OUT | OUT | OUT | OUT |
    // Pullup:      |  0  |  0  |  0  |  0  ||  0  |  0  |  0  |  0  |
    DDRB  = 0xFF;
    PORTB = 0x00;

    // PORT C
    // Direction:   |  IN |  IN | OUT || OUT | OUT | OUT | IN  |
    // Pullup:      |  0  |  0  |  0  ||  0  |  0  |  0  |  1  |
    DDRC  = 0x1E;
    PORTC = 0x01;

    // PORT D
    // Direction:   |  IN | OUT |  IN |  IN || OUT | OUT | OUT | OUT |
    // Pullup:      |  1  |  1  |  1  |  1  ||  0  |  0  |  0  |  0  |
    DDRD  = 0x4F;
    PORTD = 0xF0;
}