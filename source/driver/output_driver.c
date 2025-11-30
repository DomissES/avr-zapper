/**
 * @file output_driver.c
 * @addtogroup Level_2_Driver
 *
 * @brief Source file for output frequency driver
 *
 * @author domis
 * @date 16.11.2025
 */

// File specific includes
#include "output_driver.h"
#include "output_driver_table.h"

#include "global_defines.h"
#include "system_settings.h"

#include "adc.h"
#include "logging.h"
#include "timer_hal.h"

// Target specific includes
#include <stdlib.h>
#include <util/atomic.h>

//===================================================================================================================//
// Private macro defines                                                                                             //
//===================================================================================================================//

// Used prescaler is 32
#define OCR_CONSTANT         (F_CPU / (2 * 32))

#define CONTROL_INPUT_OFFSET 25
//===================================================================================================================//
// Private definitions                                                                                               //
//===================================================================================================================//

typedef struct
{
    uint16_t controlInputSamples[OUTPUT_DRIVER_CONTROL_IN_SAMPLES];
    uint8_t  setFrequency;
    uint16_t rawControlInput;
    uint16_t controlInput;
    bool     enabled;
} OutputDriver_t;

//===================================================================================================================//
// Private variables                                                                                                 //
//===================================================================================================================//

OutputDriver_t hOutput;

//===================================================================================================================//
// Private functions                                                                                                 //
//===================================================================================================================//

static void OutputDriver_privSetTimerPrescaler(uint8_t prescaler)
{
    TimerHAL_SetPrescaler(eTIMER_2, prescaler);
    if(TimerHAL_IsTimerEnabled(eTIMER_2))
    {
        TimerHAL_StartTimer(eTIMER_2);
    }
}

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief If possible, adds measurement to averaging process.
 *
 *
 * @param measurement Input value to be processed
 */
void OutputDriver_PerformControlInput(uint16_t measurement)
{
    static uint8_t position = 0;

    hOutput.controlInputSamples[position] = measurement - CONTROL_INPUT_OFFSET;

    position = (position + 1) % OUTPUT_DRIVER_CONTROL_IN_SAMPLES;
}

/**
 * @brief Initializes Output driver.
 *
 * Timer 1 must be initialized before this function call.
 *
 */
void OutputDriver_Init()
{
    TimerHAL_StartTimer(eTIMER_2);
}

/**
 * @brief Enables output
 *
 */
void OutputDriver_Enable()
{
    TIMER_HAL_ENABLE_OCR2();
    hOutput.enabled = true;
}

/**
 * @brief Disables output
 *
 */
void OutputDriver_Disable()
{
    TIMER_HAL_DISABLE_OCR2();
    hOutput.enabled = false;
}

/**
 * @brief Set the frequency based on the input parameter.
 *
 * This function calculates proper frequenmcy out from the timer and cpu clock (it's not universal calculator though)
 * and sets it. If the input parameter equals 0, then the frequency is taken from control input.
 *
 * @param frequency to be set, set to 0 if the frequency should be taken from control input
 */
void OutputDriver_SetFrequency(uint16_t frequency)
{
    uint32_t ocr;

    if(frequency == 0)
    {
        frequency = OutputDriver_GetControlInput();
    }
    if(frequency < OUTPUT_DRIVER_MIN_FREQUENCY)
    {
        frequency = OUTPUT_DRIVER_MIN_FREQUENCY;
    }
    if(frequency > OUTPUT_DRIVER_MAX_FREQUENCY)
    {
        frequency = OUTPUT_DRIVER_MAX_FREQUENCY;
    }

    // ========================================================================
    ASSERT((frequency < 1) || (frequency > OUTPUT_DRIVER_TABLE_SIZE));

    hOutput.setFrequency = frequency;

    TimerHAL_SetOCR(eTIMER_2, outputDriver_table[frequency - 1][0]);
    OutputDriver_privSetTimerPrescaler(outputDriver_table[frequency - 1][1]);

    LOG_DEBUG(
        "OCR is:\t %d, prescaler is: %d", outputDriver_table[frequency - 1][0], outputDriver_table[frequency - 1][1]);
}

/**
 * @brief Returns average of the control input potentiometer.
 *
 * @return Value from potentiometer from 0 to 1000
 */
uint16_t OutputDriver_GetControlInput()
{
    uint16_t voltage;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        uint32_t accum = 0;

        for(uint8_t i = 0; i < OUTPUT_DRIVER_CONTROL_IN_SAMPLES; i++)
        {
            accum += hOutput.controlInputSamples[i];
        }

        hOutput.rawControlInput = accum / OUTPUT_DRIVER_CONTROL_IN_SAMPLES;
    }
    LOG_DEBUG("Raw control input is: %d", hOutput.rawControlInput);
    hOutput.controlInput = (ADC_MAX_RESOLUTION - CONTROL_INPUT_OFFSET - hOutput.rawControlInput);

    return hOutput.controlInput;
}
