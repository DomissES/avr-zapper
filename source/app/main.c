/**
 * @file main.c
 * @addtogroup Level_TODO::
 *
 * @brief Source file for main function.
 *
 * @author domis
 * @date 30.10.2025
 */

// File specific includes
#include "global_defines.h"
#include "system_settings.h"

#include "adc.h"
#include "dcdc_driver.h"
#include "display_driver.h"
#include "gpio.h"
#include "logging.h"
#include "output_driver.h"
#include "timer_hal.h"
#include "uart_hal.h"
#include "uart_printf.h"

// Target specific includes
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

//===================================================================================================================//
// Public macro defines                                                                                              //
//===================================================================================================================//

//===================================================================================================================//
// Public definitions                                                                                                //
//===================================================================================================================//

//===================================================================================================================//
// Public variables                                                                                                  //
//===================================================================================================================//

Adc_Channel_t g_channel[2];

typedef enum
{
    eMAIN_STATE_INIT,
    eMAIN_SHOW_LOW_BAT,
    eMAIN_STATE_SELECT_FREQ,
    eMAIN_STATE_SHOW_VOLTAGE,
    eMAIN_STATE_WORK,
    eMAIN_STATE_ERROR,
} Main_states_e;

Main_states_e mainState = eMAIN_STATE_INIT;
Main_states_e previousState = eMAIN_STATE_INIT;

//===================================================================================================================//
// App function declarations                                                                                         //
//===================================================================================================================//

static Main_states_e Main_Init();
static Main_states_e Main_ShowLowBattery();
static Main_states_e Main_SelectFreq();
static Main_states_e Main_ShowVoltage();
static Main_states_e Main_Work();
static Main_states_e Main_Error();

//===================================================================================================================//
// main function                                                                                                  //
//===================================================================================================================//

int main(void)
{
    /**
     * Init gpio
     *      timers
     *      uart
     * test voltage
     *
     * Send something to uart
     *
     * go to idle state
     *      Select frequency
     *      Select voltage
     *
     * voltage state
     *
     * go to work state
     *      enable outputs
     *
     */

    uint8_t number = 0;

    uint16_t setVoltage = 5000;
    Adc_Perform();
    OutputDriver_SetFrequency(100);
    uint16_t tempOCR;
    OutputDriver_Enable();

    while(1)
    {
        // Main switching for application state machine
        switch(mainState)
        {
        case eMAIN_STATE_INIT:
            mainState = Main_Init();
            break;

        case eMAIN_STATE_SELECT_FREQ:
            mainState = Main_SelectFreq();
            break;

        case eMAIN_STATE_SHOW_VOLTAGE:
            mainState = Main_ShowVoltage();
            break;

        case eMAIN_STATE_WORK:
            mainState = Main_Work();
            break;

        case eMAIN_STATE_ERROR:
            mainState = Main_Error();
            break;

        default:
            mainState = eMAIN_STATE_ERROR;
            break;
        }
    }

    return 0;
}

//===================================================================================================================//
// Application help functions                                                                                        //
//===================================================================================================================//

static bool Main_HasEnteredState()
{
    return( mainState == previousState) ? true : false;
}

//===================================================================================================================//
// Main state init functions                                                                                         //
//===================================================================================================================//

static void MainInit_BlinkLed()
{
    for(uint8_t i = 0; i < 2; i++)
    {
        GPIO_OUT_LED_A_ENABLE();
        GPIO_OUT_LED_B_ENABLE();
        GPIO_OUT_LED_C_ENABLE();
        _delay_ms(250);
        
        GPIO_OUT_LED_A_DISABLE();
        GPIO_OUT_LED_B_DISABLE();
        GPIO_OUT_LED_C_DISABLE();
        _delay_ms(250);
    }
}

/**
 * @brief
 *
 * @return eMAIN_STATE_SELECT_FREQ, eMAIN_STATE_ERROR, eMAIN_SHOW_LOW_BAT
 */
static Main_states_e Main_Init()
{

    // Hal initialization
    Gpio_InitAll();
    Uart_InitUart();
    Adc_Init();
    TimerHAL_InitTimer_0(&hTimer0); // sysTimer
    TimerHAL_InitTimer_1(&hTimer1); // Dcdc Timer
    TimerHAL_InitTimer_2(&hTimer2); // Output Timer

    // Driver initialization
    LOG_DEBUG("====== APPLICATION START ======\n");
    DcdcDriver_Init();

    // Starting peripherals
    ENABLE_GLOBAL_INTERRUPTS();
    TimerHAL_StartTimer(eTIMER_0);
    DisplayDriver_SetMode(eDISPLAY_MODE_ON);

    // Small self test

    uint16_t voltage;
    voltage = Adc_CheckInputVoltage();
    LOG_DEBUG("Input voltage: %u mV", voltage);

    if(voltage < BATTERY_LOW_THRESHOLD)
    {
        return eMAIN_SHOW_LOW_BAT;
    }

    // Test is Ok;
    MainInit_BlinkLed();
    return eMAIN_STATE_SELECT_FREQ;

}


//===================================================================================================================//
// Application functions                                                                                             //
//===================================================================================================================//

/**
 * @brief
 *
 * @return eMAIN_STATE_ERROR
 */
static Main_states_e Main_ShowLowBattery() {}


//===================================================================================================================//
// Application functions                                                                                             //
//===================================================================================================================//

/**
 * @brief
 *
 * @return eMAIN_STATE_SELECT_FREQ, eMAIN_STATE_SHOW_VOLTAGE, eMAIN_STATE_WORK, eMAIN_STATE_ERROR
 */
static Main_states_e Main_SelectFreq()
{


    // Go to work

    // Change output voltage
}


//===================================================================================================================//
// Application functions                                                                                             //
//===================================================================================================================//

/**
 * @brief
 *
 * @return eMAIN_STATE_SELECT_FREQ,
 */
static Main_states_e Main_ShowVoltage() {}


//===================================================================================================================//
// Application functions                                                                                             //
//===================================================================================================================//

/**
 * @brief
 *
 * @return eMAIN_STATE_WORK, eMAIN_STATE_SELECT_FREQ, eMAIN_STATE_ERROR
 */
static Main_states_e Main_Work()
{
    DcdcDriver_Enable(true);
    OutputDriver_Init();
}


//===================================================================================================================//
// Application functions                                                                                             //
//===================================================================================================================//

/**
 * @brief
 *
 * @return eMAIN_STATE_ERROR
 */
static Main_states_e Main_Error() {}