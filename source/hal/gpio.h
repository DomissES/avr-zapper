/**
 * @file gpio.h
 * @addtogroup Level_1_HAL
 * @brief Header file for GPIO (General Purpose Input/Output) HAL (Hardware Abstraction Layer).
 *
 * This file provides the interface for GPIO operations, including initialization,
 * configuration, and control of GPIO pins.
 *
 * @author domis
 * @date 09.09.2024
 */

#ifndef GPIO_H_
#define GPIO_H_

// File specific includes
#include "pinConfig.h"

#include "global_defines.h"
#include "system_settings.h"

// Target specific includes
#include <avr/io.h>

//===================================================================================================================//
// Public typedefs                                                                                                   //
//===================================================================================================================//

typedef enum
{
    eGPIO_BUTTON_A,
    eGPIO_BUTTON_B
} Gpio_ButtonInstance_e;
typedef enum
{
    eBUTTON_STATUS_IDLE,
    eBUTTON_STATUS_PRESSED,
    eBUTTON_STATUS_RELEASED
} Gpio_ButtonStatus_e;

typedef volatile struct
{
    uint8_t oldValue;
    uint8_t debouncedValue;
    uint8_t counter;
    bool counting;
} Gpio_Debouncer_t;
typedef volatile struct
{
    Gpio_Debouncer_t    debounce;
    Gpio_ButtonStatus_e status;
    bool                input;
} Gpio_Button_t;

//===================================================================================================================//
// Public variables                                                                                                  //
//===================================================================================================================//

extern Gpio_Button_t hButton[2];
//===================================================================================================================//
// Macro for pin manipulation                                                                                        //
//===================================================================================================================//

#define GLUE(a, b)                a##b
#define SET_(what, p, m)          GLUE(what, p) |= (1 << (m))
#define SET(what, x)              SET_(what, x)
#define GET_(/*PIN*/ p, m)        GLUE(PIN, p) & (1 << (m))
#define GET(/*PIN*/ x)            GET_(x)
#define CLR_(what, p, m)          GLUE(what, p) &= ~(1 << (m))
#define CLR(what, x)              CLR_(what, x)
#define TOGGLE_(what, p, m)       GLUE(what, p) ^= (1 << (m))
#define TOGGLE(what, x)           TOGGLE_(what, x)

//===================================================================================================================//
// GPIO configuration macros                                                                                         //
//===================================================================================================================//

// PORT B

// OUT, SEG_LEFT
#define GPIO_SEG_LEFT_ENABLE()    SET(PORT, SEG_LEFT)
#define GPIO_SEG_LEFT_DISABLE()   CLR(PORT, SEG_LEFT)

// OUT, SEG_RIGHT
#define GPIO_SEG_RIGHT_ENABLE()   SET(PORT, SEG_RIGHT)
#define GPIO_SEG_RIGHT_DISABLE()  CLR(PORT, SEG_RIGHT)

// OUT, OUT_KEY
#define GPIO_OUT_KEY_ENABLE()     SET(PORT, OUT_KEY)
#define GPIO_OUT_KEY_DISABLE()    CLR(PORT, OUT_KEY)

// OUT, DC_DC_KEY
#define GPIO_OUT_DC_DC_ENABLE()   SET(PORT, DC_DC_KEY)
#define GPIO_OUT_DC_DC_DISABLE()  CLR(PORT, DC_DC_KEY)

// OUT, LED_A
#define GPIO_OUT_LED_A_ENABLE()   CLR(PORT, LED_A)
#define GPIO_OUT_LED_A_DISABLE()  SET(PORT, LED_A)

// OUT, LED_B
#define GPIO_OUT_LED_B_ENABLE()   CLR(PORT, LED_B)
#define GPIO_OUT_LED_B_DISABLE()  SET(PORT, LED_B)

// OUT, LED_C
#define GPIO_OUT_LED_C_ENABLE()   CLR(PORT, LED_C)
#define GPIO_OUT_LED_C_DISABLE()  SET(PORT, LED_C)

// PORT C

// OUT, SEG_A
#define GPIO_OUT_SEG_A_ENABLE()   CLR(PORT, SEG_A)
#define GPIO_OUT_SEG_A_DISABLE()  SET(PORT, SEG_A)

// OUT, SEG_B
#define GPIO_OUT_SEG_B_ENABLE()   CLR(PORT, SEG_B)
#define GPIO_OUT_SEG_B_DISABLE()  SET(PORT, SEG_B)

// OUT, SEG_C
#define GPIO_OUT_SEG_C_ENABLE()   CLR(PORT, SEG_C)
#define GPIO_OUT_SEG_C_DISABLE()  SET(PORT, SEG_C)

// OUT, SEG_D
#define GPIO_OUT_SEG_D_ENABLE()   CLR(PORT, SEG_D)
#define GPIO_OUT_SEG_D_DISABLE()  SET(PORT, SEG_D)

// PORT D

// IN, SWITCH_A
#define GPIO_IN_GET_SWITCH_A()    GET(SWITCH_A)

// IN, SWITCH_B
#define GPIO_IN_GET_SWITCH_B()    GET(SWITCH_B)

// OUT, SEG_E
#define GPIO_OUT_SEG_E_ENABLE()   CLR(PORT, SEG_E)
#define GPIO_OUT_SEG_E_DISABLE()  SET(PORT, SEG_E)

// OUT, SEG_F
#define GPIO_OUT_SEG_F_ENABLE()   CLR(PORT, SEG_F)
#define GPIO_OUT_SEG_F_DISABLE()  SET(PORT, SEG_F)

// OUT, SEG_G
#define GPIO_OUT_SEG_G_ENABLE()   CLR(PORT, SEG_G)
#define GPIO_OUT_SEG_G_DISABLE()  SET(PORT, SEG_G)

// OUT, SEG_DP
#define GPIO_OUT_SEG_DP_ENABLE()  CLR(PORT, SEG_DP)
#define GPIO_OUT_SEG_DP_DISABLE() SET(PORT, SEG_DP)

//===================================================================================================================//
// Public Functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Performs debouncing of buttons.
 *
 * At best run it periodically.
 *
 */
void Gpio_ButtonsPerform();

/**
 * @brief Initializes all GPIO pins and sets their initial states.
 *
 *  This function is specific to current hardware configuration. Refer to the documentation for details.
 *
 * @param[in]  None
 * @param[out] None
 */
void Gpio_InitAll();

/**
 * @brief Return current status of button
 *
 * Available statuses are Pressed, Release, Idle. If it was pressed or release and this function was called, the status
 * gets back to idle. It takes the debounced value.
 *
 * @param pButton pointer to the Button object.
 * @return Idle, Pressed, Released.
 */
Gpio_ButtonStatus_e Gpio_GetButton(Gpio_Button_t *pButton);

/**
 * @brief Function which enables sequentally all leds to check if they are all working
 *
 */
void Gpio_TestAll();

#endif // GPIO_H_