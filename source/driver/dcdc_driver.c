/**
 * @file dcdc_driver.c
 * @addtogroup Level_2_Driver
 *
 * @brief Source file for Dc Dc step up converter
 * @author domis
 * @date 07.11.2025
 */

// File specific includes
#include "dcdc_driver.h"

#include "global_defines.h"
#include "system_settings.h"

#include "adc.h"
#include "logging.h"
#include "timer_hal.h"

// Target specific includes
#include <util/atomic.h>

//===================================================================================================================//
// Private macro defines                                                                                             //
//===================================================================================================================//

#define DCDC_OUTPUT_SEQUENCE_LENGTH 4
//===================================================================================================================//
// Private definitions                                                                                               //
//===================================================================================================================//

typedef struct
{
    uint32_t accum;
    uint8_t  samples;
    uint16_t missedSamples;
} DcdcDriver_Averaging_t;

typedef struct
{
    uint8_t  seq[DCDC_OUTPUT_SEQUENCE_LENGTH];
    uint8_t  position;
    uint8_t  dutyCycle;
    uint16_t raw;
} DcdcDriver_OutControl_t;

typedef struct dcdc_driver
{
    uint16_t                setVoltage;
    uint16_t                actualVoltage;
    uint16_t                actualRawVoltage;
    DcdcDriver_Averaging_t  averaging;
    DcdcDriver_OutControl_t output;
} Dcdc_Driver_t;

//===================================================================================================================//
// Private variables                                                                                                 //
//===================================================================================================================//

Dcdc_Driver_t hDcdc;

//===================================================================================================================//
// Private functions                                                                                                  //
//===================================================================================================================//

static uint16_t DcdcDriver_privConvertVoltage(uint16_t rawVoltage)
{
    // Added +1 for divide optimization
    uint32_t temp = ((uint32_t)rawVoltage * ADC_VREF) / (ADC_MAX_RESOLUTION + 1);
    return DCDC_INPUT_VOLTAGE_DIVIDER * (uint16_t)temp;
}

static uint8_t DcdcDriver_privRegulateOutput(uint16_t setVoltage, uint16_t actualVoltage)
{
    int16_t        error = actualVoltage - setVoltage;
    static int16_t integral;
    integral += error;

    if(integral > 2048)
    {
        integral = 2048;
    }
    if(integral < -2048)
    {
        integral = -2048;
    }

    int16_t output = -((error / 32) + (integral / 512)); // simple P controller with Kp = 0.01

    if(output > 4 * DCDC_TIMER_MAX_OCR)
        output = 4 * DCDC_TIMER_MAX_OCR;
    if(output < 4 * DCDC_TIMER_MIN_OCR)
        output = 4 * DCDC_TIMER_MIN_OCR;

    return (uint8_t)output;
}

static void DcdcDriver_privConvertOutputToSequence(uint8_t dutyCycle)
{

    uint8_t common    = dutyCycle / DCDC_OUTPUT_SEQUENCE_LENGTH;
    uint8_t remainder = dutyCycle % DCDC_OUTPUT_SEQUENCE_LENGTH;

    for(uint8_t i = 0; i < DCDC_OUTPUT_SEQUENCE_LENGTH; i++)
    {
        uint8_t temp        = remainder > i ? 1 : 0;
        hDcdc.output.seq[i] = common + temp;
    }
}

uint8_t DcdcDriver_privPerformOutSequence()
{
    uint8_t ocrValue;

    if(hDcdc.output.position < DCDC_OUTPUT_SEQUENCE_LENGTH)
    {
        ocrValue = hDcdc.output.seq[hDcdc.output.position];
    }
    else
    {
        ocrValue = 0;
    }
    hDcdc.output.dutyCycle = ocrValue;
    hDcdc.output.position++;
    if(hDcdc.output.position >= 2 * DCDC_OUTPUT_SEQUENCE_LENGTH)
    {
        hDcdc.output.position = 0;
    }

    return hDcdc.output.dutyCycle;
}

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief If possible, adds measurement to averaging process.
 *
 * If there are enough samples, the function omits new samples until the value is processed in DcDcDriver_Perform.
 *
 * @param measurement Input value to be processed
 */
void DcdcDriver_ProcessMeasurement(uint16_t measurement)
{

    if(hDcdc.averaging.samples < DCDC_TIMER_AVERAGING_SAMPLES)
    {
        hDcdc.averaging.accum += measurement;
        hDcdc.averaging.samples++;
    }
    else
    {
        hDcdc.averaging.missedSamples++;
    }
}

/**
 * @brief Initializes DCDC converter
 *
 * HAL peripherals (timer, adc) must be initialized separately. If one of it is unitialized, this funtion return false.
 *
 */
bool DcdcDriver_Init()
{
    if((hTimer1.instance == eTIMER_NONE) || (!Adc_IsInitialized()))
    {
        return false;
    }
    TimerHAL_SetOCR(eTIMER_1, DCDC_TIMER_MIN_OCR);

    hDcdc.setVoltage       = 5000;
    hDcdc.output.dutyCycle = DCDC_TIMER_MIN_OCR;

    return true;
}

void DcdcDriver_Enable(bool on)
{
    if(on)
    {
        TimerHAL_StartTimer(eTIMER_1);
        TIMER_HAL_ENABLE_OCR1();
    }
    else
    {
        TimerHAL_StopTimer(eTIMER_1);
        TIMER_HAL_DISABLE_OCR1();
    }
}

void DcdcDriver_SetVoltage(uint16_t voltageLevel)
{
    if(voltageLevel > DCDC_MAX_OUTPUT_VOLTAGE)
    {
        voltageLevel = DCDC_MAX_OUTPUT_VOLTAGE;
    }
    if(voltageLevel < DCDC_MIN_OUTPUT_VOLTAGE)
    {
        voltageLevel = DCDC_MIN_OUTPUT_VOLTAGE;
    }
    hDcdc.setVoltage = voltageLevel;
}

uint16_t DcdcDriver_GetVoltage()
{
    return hDcdc.actualVoltage;
}

void DcdcDriver_Perform()
{
    static uint16_t voltageOutOfRange;

    // get actual voltage
    if(hDcdc.averaging.samples == DCDC_TIMER_AVERAGING_SAMPLES)
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            // Accum value is firstly divided, then reset to fit into the 32 bit type.
            // The other way would be to cast the types.
            hDcdc.averaging.accum /= DCDC_TIMER_AVERAGING_SAMPLES;
            hDcdc.actualRawVoltage  = (uint16_t)hDcdc.averaging.accum;
            hDcdc.averaging.samples = 0;
            hDcdc.averaging.accum   = 0;
        }
        LOG_DEBUG("DCDC Missed samples: %d", hDcdc.averaging.missedSamples);
        LOG_DEBUG("Raw voltage is: %d", hDcdc.actualRawVoltage);
        hDcdc.averaging.missedSamples = 0;
        hDcdc.actualVoltage           = DcdcDriver_privConvertVoltage(hDcdc.actualRawVoltage);
    }

    // set proper duty cycle or error
    if((hDcdc.actualVoltage < (hDcdc.setVoltage - DCDC_POSSIBLE_HYSTERESIS)) || // Voltage is to low
       (hDcdc.actualVoltage > (hDcdc.setVoltage + DCDC_POSSIBLE_HYSTERESIS)))   // Voltage is to high
    {
        hDcdc.output.raw = DcdcDriver_privRegulateOutput(hDcdc.setVoltage, hDcdc.actualVoltage);
        DcdcDriver_privConvertOutputToSequence(hDcdc.output.raw);
        voltageOutOfRange++;
    }
    else // Voltage is ok
    {
        voltageOutOfRange = 0;
    }
    DcdcDriver_privPerformOutSequence();
    TimerHAL_SetOCR(eTIMER_1, hDcdc.output.dutyCycle);

    LOG_DEBUG("Current duty cycle is: %d\t out of range: %d", hDcdc.output.raw, voltageOutOfRange);
}