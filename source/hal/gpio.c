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

#include "global_defines.h"
#include "system_settings.h"

// Target specific includes
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define GPIO_INPUT_BUTTON_LEVEL_ON  0
#define GPIO_INPUT_BUTTON_LEVEL_OFF 1

//===================================================================================================================//
// Private variables                                                                                                 //
//===================================================================================================================//

Gpio_Button_t button[2];

//===================================================================================================================//
// Interrupt vectors                                                                                                 //
//===================================================================================================================//

ISR(INT0_vect)
{

    button[0].input = GPIO_IN_GET_SWITCH_A();
}

ISR(INT1_vect)
{
    button[1].input = GPIO_IN_GET_SWITCH_A();
}

//===================================================================================================================//
// Private functions                                                                                                 //
//===================================================================================================================//

/**
 * @brief Debounces a level given in the input object.
 *
 * The debounce time is configured in GPIO_DEBOUNCE_TIME
 *
 * @related GPIO_DEBOUNCE_TIME
 * @param pButton pointer to the debounced object structure
 * @param currentValue current (at this moment) value to be debounced
 * @return true if input is valid, false otherwise
 */
static bool Gpio_privDebounce(Gpio_Debouncer_t *pButton, uint8_t currentValue)
{
    bool isValid = false;

    if(currentValue != pButton->oldValue)
    {
        pButton->counter = GPIO_DEBOUNCE_TIME;
    }

    if(pButton->counter == 0)
    {
        pButton->debouncedValue = pButton->oldValue;
        isValid                 = true;
    }
    else
    {
        pButton->counter--;
    }

    return isValid;
}

//===================================================================================================================//
// Public Functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Performs debouncing of buttons.
 *
 * At best run it periodically.
 *
 */
void Gpio_ButtonsPerform()
{
    if(Gpio_privDebounce(&button[0].debounce, button[0].input))
    {
        button[0].status =
            (button[0].input == GPIO_INPUT_BUTTON_LEVEL_ON) ? eBUTTON_STATUS_PRESSED : eBUTTON_STATUS_RELEASED;
    }
    if(Gpio_privDebounce(&button[1].debounce, button[1].input))
    {
        button[1].status =
            (button[1].input == GPIO_INPUT_BUTTON_LEVEL_ON) ? eBUTTON_STATUS_PRESSED : eBUTTON_STATUS_RELEASED;
    }
}

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
    // Initialize all gpios

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

    // Initialize interrupts for buttons
    MCUCR |= _BV(ISC10) | _BV(ISC00);
    GICR |= _BV(INT0) | _BV(INT1);
}

/**
 * @brief Return current status of button
 *
 * Available statuses are Pressed, Release, Idle. If it was pressed or release and this function was called, the status
 * gets back to idle. It takes the debounced value.
 *
 * @param pButton pointer to the Button object.
 * @return Idle, Pressed, Released.
 */
Gpio_ButtonStatus_e Gpio_GetButton(Gpio_Button_t *pButton)
{
    Gpio_ButtonStatus_e current = pButton->status;

    // if it was active status, reset it
    if(pButton->status != eBUTTON_STATUS_IDLE)
        pButton->status = eBUTTON_STATUS_IDLE;

    return current;
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
