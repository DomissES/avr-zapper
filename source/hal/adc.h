/**
 * @file adc.h
 * @addtogroup Level_1_HAL
 *
 * @brief Header file for adc converter
 * TODO:: add more sophisticated description and explanation
 *
 * @author domis
 * @date 07.11.2025
 */

#ifndef ADC_H_
#define ADC_H_

// File specific includes
#include "global_defines.h"
#include "system_settings.h"

// Target specific includes

//===================================================================================================================//
// Public macro defines                                                                                              //
//===================================================================================================================//

#define ADC_CHANNELS 2

//===================================================================================================================//
// Public definitions                                                                                                //
//===================================================================================================================//

typedef enum
{
    eADC_CHANNEL_DC_DC_FB = 0,
    eADC_CHANNEL_CONTROL_IN = 1,
    eADC_CHANNEL_NONE = 0xFF
} Adc_Instance_e;

typedef enum
{
    eADC_STATUS_UNITIALIZED,
    eADC_STATUS_CHECK_VOLTAGE,
    eADC_STATUS_IDLE,
    eADC_STATUS_MEASURING,
    eADC_STATUS_READY,
    eADC_STATUS_ERROR
} Adc_Status_e;

typedef struct
{
    Adc_Instance_e channel;
    uint16_t       lastMeasurement;
} Adc_Channel_t;

//===================================================================================================================//
// Public variables                                                                                                  //
//===================================================================================================================//

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Starts measure process for ADC
 */
#define ADC_START_MEASURE() ADCSRA |= _BV(ADSC)

/**
 * @brief Stops measure process for ADC
 */
#define ADC_STOP_MEASURE()  ADCSRA &= ~_BV(ADSC)

/**
 * @brief Callback function for adc ISR.
 *
 * @param channel channel which the measure was done
 */
void Adc_Done_Callback();

/**
 * @brief Initializes the ADC with configuration needed for this project
 *
 */
void Adc_Init();

/**
 * @brief Checks if adc is initialized
 * 
 * @return true if initialized, false otherwise
 */
 bool Adc_IsInitialized();

/**
 * @brief Realizes the ADC measurement.
 *
 * It selects the next channel and starts the measure. Use this function to enable continuous measuring.
 *
 * @return true
 * @return false
 */
bool Adc_Perform();

/**
 * @brief Checks the input voltage by measuring the internal bandgap reference.
 *
 * This function is intended to use only once, at system initialization.
 *
 * @return battery voltage in milivolts
 */
uint16_t Adc_CheckInputVoltage();

#endif // ADC_H_