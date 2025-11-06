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

// Target specific includes
#include <avr/io.h>

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
#define GPIO_IN_GET_SWITCH_A()    GET(PIN, SWITCH_A)

// IN, SWITCH_B
#define GPIO_IN_GET_SWITCH_B()    GET(PIN, SWITCH_B)

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
 * @brief Initializes all GPIO pins and sets their initial states.
 *
 *  This function is specific to current hardware configuration. Refer to the documentation for details.
 *
 * @param[in]  None
 * @param[out] None
 */
void Gpio_InitAll();

/**
 * @brief Function which enables sequentally all leds to check if they are all working
 * 
 */
void Gpio_TestAll();

#endif // GPIO_H_