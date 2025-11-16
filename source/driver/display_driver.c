/**
 * @file display_driver.c
 * @addtogroup Level_2_Diver
 *
 * @brief Header file for Display driver
 *
 * Display is in 2nd layer, although it hardly uses any hal underneath it.
 *
 * @author domis
 * @date 15.11.2025
 */

// File specific includes
#include "display_driver.h"

#include "global_defines.h"
#include "system_settings.h"

#include "gpio.h"
#include "timer_hal.h"

// Target specific includes
#include <avr/io.h>

//===================================================================================================================//
// Private macro defines                                                                                             //
//===================================================================================================================//

#define DISPLAY_SPECIAL_L_INDEX 16
#define DISPLAY_SPECIAL_o_INDEX 17
#define DISPLAY_SPECIAL_r_INDEX 18
//===================================================================================================================//
// Private definitions                                                                                               //
//===================================================================================================================//

typedef struct
{

    DisplayDriver_Mode_e activeMode;
    uint8_t              activeDigits[2];
    bool                 powered;
} DisplayDriver_t;

//===================================================================================================================//
// Private variables                                                                                                 //
//===================================================================================================================//

const uint8_t DisplayDriver_7SegmentMap[19] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001, // F
    0b00111000, // L
    0b01011100, // o
    0b01010000  // r
};

DisplayDriver_t display;
//===================================================================================================================//
// Private functions                                                                                                 //
//===================================================================================================================//

void DisplayDriver_privSetGpioDigit(uint8_t digit)
{
    PORTC = (PORTC & 0xC3) | ~((DisplayDriver_7SegmentMap[digit] & 0x0F) << 2);
    PORTD = (PORTD & 0x0F) | ~((DisplayDriver_7SegmentMap[digit] & 0xF0));
}

bool DisplayDriver_privToggleBlinking()
{
    static uint16_t timer;

    if((timer >= DISPLAY_BLINKING_PERIOD))
    {
        display.powered ^= 1;
        timer = 0;
    }
    timer++;
    return display.powered;
}

bool DisplayDriver_privGetPowerMode()
{
    switch(display.activeMode)
    {
    case eDISPLAY_MODE_OFF:
        return 0;
    case eDISPLAY_MODE_BLINKING:
        return DisplayDriver_privToggleBlinking();
    case eDISPLAY_MODE_ON:
        return 1;
    }

    return 0;
}

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

void DisplayDriver_PerformMultiplex()
{
    static uint8_t activeSegment;

    // toggle digit
    activeSegment ^= 1;

    // set proper digit (cathode)
    DisplayDriver_privSetGpioDigit(display.activeDigits[activeSegment]);

    // set proper source (anode)
    if(DisplayDriver_privGetPowerMode())
    {
        if(activeSegment) // right segment
        {
            GPIO_SEG_LEFT_DISABLE();
            GPIO_SEG_RIGHT_ENABLE();
        }
        else // left segment
        {
            GPIO_SEG_RIGHT_DISABLE();
            GPIO_SEG_LEFT_ENABLE();
        }
    }
    else
    {
        GPIO_SEG_LEFT_DISABLE();
        GPIO_SEG_RIGHT_DISABLE();
    }
}

/**
 * @brief This function checks if any of needed peripherals are enabled.
 * All the peripherals shall be initialized before display driver.
 *
 */
void DisplayDriver_Init() {}

void DisplayDriver_SetMode(DisplayDriver_Mode_e mode)
{
    display.activeMode = mode;
}

void DisplayDriver_SetNumber(uint8_t number)
{
    // split the number into two digits
    display.activeDigits[0] = number / 10;
    display.activeDigits[1] = number % 10;
}

void DisplayDriver_SetSpecial(DisplayDriver_SpecialDigit_e digit)
{
    switch(digit)
    {
    case eDISPLAY_DIGIT_ERROR:
        display.activeDigits[0] = 0x0E;                    // E
        display.activeDigits[1] = DISPLAY_SPECIAL_r_INDEX; // r
        break;
    case eDISPLAY_DIGIT_BATTERY:
        display.activeDigits[0] = 0x0B; // B
        display.activeDigits[1] = 0x0A; // A
        break;
    case eDISPLAY_DIGIT_BATTERY_LOW:
        display.activeDigits[0] = DISPLAY_SPECIAL_L_INDEX; // L
        display.activeDigits[1] = DISPLAY_SPECIAL_o_INDEX; // o
        break;
    default:
        break;
    }
}
