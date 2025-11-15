/**
 * @file callbacks.c
 * @addtogroup Level_x_App
 *
 * @brief Source file for all callbacks functions
 *
 * @author domis
 * @date 12.11.2025
 */

// File specific includes
#include "global_defines.h"
#include "system_settings.h"

#include "adc.h"
#include "dcdc_driver.h"
#include "timer_hal.h"

// Target specific includes
#include <avr/interrupt.h>

//===================================================================================================================//
// Private macro defines                                                                                             //
//===================================================================================================================//

//===================================================================================================================//
// Private definitions                                                                                               //
//===================================================================================================================//


//===================================================================================================================//
// Function callbacks                                                                                                //
//===================================================================================================================//

void Adc_Done_Callback(volatile Adc_Channel_t *channel)
{
    if (channel->channel == eADC_CHANNEL_CONTROL_IN)
    {
        /* code */
    }
    else
    {
        DcdcDriver_ProcessMeasurement(channel->lastMeasurement);
    }
}

void TimerHAL_Timer0_OverflowCallback()
{
    DisplayDriver_PerformMultiplex();
}