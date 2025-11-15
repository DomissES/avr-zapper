/**
 * @file adc.c
 * @addtogroup Level_1_HAL
 *
 * @brief Source file for adc
 * This project uses two channels of adc, one for control in and one for dcdc. This module provides basic initialization
 * and measurement functions.
 * Additional function is the check voltage function. It is meant to call it only at the very beginning of whole application.
 *
 * @author domis
 * @date 07.11.2025
 */

// File specific includes
#include "adc.h"

#include "global_defines.h"
#include "system_settings.h"

// Target specific includes
#include <avr/interrupt.h>
#include <avr/io.h>

//===================================================================================================================//
// Private macro defines                                                                                             //
//===================================================================================================================//

#define ADC_PRESCALER_2    1
#define ADC_PRESCALER_4    2
#define ADC_PRESCALER_8    3
#define ADC_PRESCALER_16   4
#define ADC_PRESCALER_32   5
#define ADC_PRESCALER_64   6
#define ADC_PRESCALER_128  7

#define ADC_USED_PRESCALER ADC_PRESCALER_128

#define ADC_CHANNEL_1      0
#define ADC_CHANNEL_2      1

#define ADC_SET_CHANNEL_1  ADCSRA = (ADCSRA & 0xF0) | ADC_CHANNEL_1
#define ADC_SET_CHANNEL_2  ADCSRA = (ADCSRA & 0xF0) | ADC_CHANNEL_2

// clang-format off
#define ADC_HAL_INIT_STRUCT()                                                                                         \
{                                                                                                                     \
    .channel[0] = &adc_DcdcChannel,                                                                                   \
    .channel[1] = &adc_ControlChannel,                                                                                \
    .status = eADC_STATUS_UNITIALIZED,                                                                                \
    .activeChannel = eADC_CHANNEL_DC_DC_FB,                                                                           \
} 

#define ADC_CHANNEL_INIT_STRUCT()                                                                                     \
{                                                                                                                     \
    .channel = eADC_CHANNEL_NONE,                                                                                     \
    .lastMeasurement = 0                                                                                              \
}
// clang-format on

//===================================================================================================================//
// Private definitions                                                                                               //
//===================================================================================================================//

typedef struct
{
    Adc_Channel_t *channel[ADC_CHANNELS];
    Adc_Status_e   status;
    Adc_Instance_e activeChannel;
} Adc_HAL_t;

//===================================================================================================================//
// Private variables                                                                                                 //
//===================================================================================================================//

Adc_Channel_t adc_DcdcChannel    = ADC_CHANNEL_INIT_STRUCT();
Adc_Channel_t adc_ControlChannel = ADC_CHANNEL_INIT_STRUCT();

volatile Adc_HAL_t hAdc = ADC_HAL_INIT_STRUCT();
volatile uint16_t  batteryVoltage;

//===================================================================================================================//
// ISR functions                                                                                                     //
//===================================================================================================================//

/**
 * @brief Callback function for adc ISR.
 *
 * @param channel channel which the measure was done
 */
__weak void Adc_Done_Callback(volatile Adc_Channel_t *channel)
{
    (void)channel;
}

/**
 * @brief ISR function
 */
ISR(ADC_vect)
{

    if(hAdc.status == eADC_STATUS_CHECK_VOLTAGE)
    {
        batteryVoltage = ADC;
    }
    else
    {
        hAdc.status                                       = eADC_STATUS_READY;
        hAdc.channel[hAdc.activeChannel]->lastMeasurement = ADC;
        Adc_Done_Callback(hAdc.channel[hAdc.activeChannel]);
        Adc_Perform();
    }

    hAdc.status = eADC_STATUS_IDLE;
}

//===================================================================================================================//
// Private functions                                                                                                 //
//===================================================================================================================//

/**
 * @brief Jumps to the next available channel for measurement
 *
 */
static void Adc_privJumpToNextChannel()
{

    if(hAdc.activeChannel == eADC_CHANNEL_CONTROL_IN)
    {
        ADMUX              = (ADMUX & 0xF0) | eADC_CHANNEL_DC_DC_FB;
        hAdc.activeChannel = eADC_CHANNEL_DC_DC_FB;
    }
    else
    {
        ADMUX              = (ADMUX & 0xF0) | eADC_CHANNEL_CONTROL_IN;
        hAdc.activeChannel = eADC_CHANNEL_CONTROL_IN;
    }
}

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Initializes the ADC with configuration needed for this project
 *
 */
void Adc_Init()
{

    if(hAdc.status == eADC_STATUS_CHECK_VOLTAGE) // Operation only at init
    {
        ADMUX = _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
        ADMUX = (ADMUX & 0x0F) | (_BV(REFS0)); // Vref to AVcc
    }
    else // Normal operation
    {
        hAdc.status = eADC_STATUS_IDLE;
        // Set voltage reference to internal
        ADMUX = _BV(REFS0) | _BV(REFS1);
        // Enable interrupts and prescaler
        ADCSRA = _BV(ADEN) | _BV(ADIE) | ADC_USED_PRESCALER;

        hAdc.channel[eADC_CHANNEL_DC_DC_FB]->channel   = eADC_CHANNEL_DC_DC_FB;
        hAdc.channel[eADC_CHANNEL_CONTROL_IN]->channel = eADC_CHANNEL_CONTROL_IN;
    }
}

/**
 * @brief Checks if adc is initialized
 *
 * @return true if initialized, false otherwise
 */
bool Adc_IsInitialized()
{
    return ((hAdc.status != eADC_STATUS_UNITIALIZED) && (hAdc.status != eADC_STATUS_CHECK_VOLTAGE));
}

/**
 * @brief Realizes the ADC measurement.
 *
 * It selects the next channel and starts the measure. Use this function to enable continuous measuring.
 *
 * @return true
 * @return false
 */
bool Adc_Perform()
{
    // if measure done, obtain measure
    // then change to new channel and start again

    // if not, start

    if((hAdc.status == eADC_STATUS_IDLE) || (hAdc.status == eADC_STATUS_READY))
    {
        Adc_privJumpToNextChannel();
        ADC_START_MEASURE();
        hAdc.status = eADC_STATUS_MEASURING;

        return true;
    }

    return false;
}

/**
 * @brief Checks the input voltage by measuring the internal bandgap reference.
 *
 * This function is intended to use only once, at system initialization.
 *
 * @return battery voltage in milivolts
 */
uint16_t Adc_CheckInputVoltage()
{
    uint16_t voltage;

    hAdc.status = eADC_STATUS_CHECK_VOLTAGE;

    // set proper admux and tweak reference
    Adc_Init();

    // start measure
    ADC_START_MEASURE();

    // wait for it
    while(hAdc.status != eADC_STATUS_IDLE)
    {
        ;
    }

    // calculate it
    voltage = (uint16_t)(((uint32_t)(ADC_VINTERNAL + ADC_INT_OFFSET) * ADC_MAX_RESOLUTION) / batteryVoltage); // in mV

    // go back to normal mode
    hAdc.status = eADC_STATUS_UNITIALIZED;
    Adc_Init();

    return voltage;
}