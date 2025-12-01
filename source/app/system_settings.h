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
#define UART_BAUDRATE                    57600

#define LOG_LEVEL                        4
#define LOG_PRINTF_FUNC(...)             UartPrintf_Printf(__VA_ARGS__)
#define UART_PRINTF_BUFFER_SIZE          96
#define LOG_SHOW_FUNCTION_LINES          1

// Adc related
// Values referred to proper measuring
#define ADC_VINTERNAL                    1300  // mV
#define ADC_INT_OFFSET                   (-85) // mV
#define ADC_MAX_RESOLUTION               1023  // mV
#define ADC_VREF                         2540  // mV

// DCDC driver related
#define DCDC_TIMER_OCR_VALUE             64

#define DCDC_TIMER_MIN_OCR               0
#define DCDC_TIMER_MAX_OCR               16
#define DCDC_TIMER_AVERAGING_SAMPLES     16

#define DCDC_INPUT_VOLTAGE_DIVIDER       12
#define DCDC_POSSIBLE_HYSTERESIS         00 // mV

#define DCDC_MAX_OUTPUT_VOLTAGE          20000 // mV
#define DCDC_MIN_OUTPUT_VOLTAGE          5000  // mV

// Display Driver related
#define DISPLAY_BLINKING_PERIOD          2048

// Gpio related
#define GPIO_DEBOUNCE_TIME               128

// Output driver related
#define OUTPUT_DRIVER_MIN_FREQUENCY      1   // kHz
#define OUTPUT_DRIVER_MAX_FREQUENCY      100 // kHz

#define OUTPUT_DRIVER_CONTROL_IN_SAMPLES 8

// Battery check related
#define BATTERY_LOW_THRESHOLD           3300 // mV

#endif // SYSTEM_SETTINGS_H_