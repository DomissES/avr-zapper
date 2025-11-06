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
#include <util/delay.h>

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
    // Direction:   | OUT | OUT | OUT | MISO || OUT | OUT | OUT | OUT |
    // Pullup:      |  1  |  1  |  1  |  0   ||  0  |  0  |  0  |  0  |
    DDRB  = 0xEF;
    PORTB = 0xE0;

    // PORT C
    // Direction:   |  -- | OUT | OUT || OUT | OUT | IN | IN |
    // Pullup:      |  0  |  1  |  1  ||  1  |  1  | 0  | 0  |
    DDRC  = 0x3C;
    PORTC = 0x3C;

    // PORT D
    // Direction:   | OUT | OUT | OUT | OUT || IN | IN | TXD | RXD |
    // Pullup:      |  1  |  1  |  1  |  1  || 1  | 1  |  1  |  0  |
    DDRD  = 0xF1;
    PORTD = 0xFE;
}

//===================================================================================================================//
// Testing Functions                                                                                                 //
//===================================================================================================================//

static void Gpio_privEnableLedAll()
{
    GPIO_OUT_LED_A_ENABLE();
    GPIO_OUT_LED_B_ENABLE();
    GPIO_OUT_LED_C_ENABLE();

    GPIO_SEG_RIGHT_ENABLE();
    GPIO_SEG_LEFT_ENABLE();

    GPIO_OUT_SEG_A_ENABLE();
    GPIO_OUT_SEG_B_ENABLE();
    GPIO_OUT_SEG_C_ENABLE();
    GPIO_OUT_SEG_D_ENABLE();

    GPIO_OUT_SEG_E_ENABLE();
    GPIO_OUT_SEG_F_ENABLE();
    GPIO_OUT_SEG_G_ENABLE();
    GPIO_OUT_SEG_DP_ENABLE();
}

static void Gpio_privDisableLedAll()
{
    PORTB |= 0xE0;
    PORTC |= 0x3C;
    PORTD |= 0xF0;

    PORTB &= ~(0x05);
}

static void Gpio_privEnableSegmentLEDs()
{
    GPIO_OUT_SEG_A_ENABLE();
    _delay_ms(150);
    GPIO_OUT_SEG_B_ENABLE();
    _delay_ms(150);
    GPIO_OUT_SEG_C_ENABLE();
    _delay_ms(150);
    GPIO_OUT_SEG_D_ENABLE();
    _delay_ms(150);

    GPIO_OUT_SEG_E_ENABLE();
    _delay_ms(150);
    GPIO_OUT_SEG_F_ENABLE();
    _delay_ms(150);
    GPIO_OUT_SEG_G_ENABLE();
    _delay_ms(150);
    GPIO_OUT_SEG_DP_ENABLE();
    _delay_ms(150);
}

/**
 * @brief Function which enables sequentally all leds to check if they are all working
 * 
 */
void Gpio_TestAll()
{
    // Test Leds
    GPIO_OUT_LED_A_ENABLE();
    _delay_ms(150);
    GPIO_OUT_LED_B_ENABLE();
    _delay_ms(150);
    GPIO_OUT_LED_C_ENABLE();
    _delay_ms(150);

    Gpio_privDisableLedAll();

    // Test Segment Left
    _delay_ms(150);
    GPIO_SEG_LEFT_ENABLE();

    Gpio_privEnableSegmentLEDs();

    // Toggle to right segment
    Gpio_privDisableLedAll();
    GPIO_SEG_RIGHT_ENABLE();
    
    Gpio_privEnableSegmentLEDs();

    Gpio_privDisableLedAll();

}
