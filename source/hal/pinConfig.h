/**
 * @file pinConfig.h
 * @addtogroup Level_0_Config
 *
 * @brief Header file for HW pin configuration.
 *
 * @author domis
 * @date 25.09.2025
 */

#ifndef PIN_CONFIG_H_
#define PIN_CONFIG_H_

//===================================================================================================================//
// Pin definitions                                                                                                   //
//===================================================================================================================//

// PORT B
#define SEG_LEFT   B, 0 // OUT
#define SEG_RIGHT  B, 1 // OUT
#define DC_DC_KEY  B, 2 // OUT
#define OUT_KEY    B, 3 // OUT
// #define MISO       B, 4
#define LED_A      B, 5 // OUT
#define LED_B      B, 6 // OUT
#define LED_C      B, 7 // OUT

// PORT C
#define DC_DC_FB   C, 0 // ADC IN
#define CONTROL_IN C, 1 // ADC IN
#define SEG_A      C, 2 // OUT
#define SEG_B      C, 3 // OUT
#define SEG_C      C, 4 // OUT
#define SEG_D      C, 5 // OUT
// #define RESET      C, 6 // IN

// PORT D
// #define UART_RX    D, 0
// #define UART_TX    D, 1
#define SWITCH_A   D, 2 // IN
#define SWITCH_B   D, 3 // IN
#define SEG_E      D, 4 // OUT
#define SEG_F      D, 5 // OUT
#define SEG_G      D, 6 // OUT
#define SEG_DP     D, 7 // OUT

#endif // PIN_CONFIG_H_