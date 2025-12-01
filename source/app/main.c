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

Main_states_e currentState = eMAIN_STATE_INIT;
Main_states_e oldState     = eMAIN_STATE_INIT;
Main_states_e nextState    = eMAIN_STATE_INIT;

uint16_t gOutputVoltage = DCDC_MIN_OUTPUT_VOLTAGE;

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

    OutputDriver_SetFrequency(100);
    uint16_t tempOCR;

    uint16_t timer        = 5; // ~ 10 ms
    uint32_t oldTimestamp = timestamp;

    while(1)
    {
        // Main switching for application state machine
        switch(currentState)
        {
        case eMAIN_STATE_INIT:
            nextState = Main_Init();
            break;

        case eMAIN_STATE_SELECT_FREQ:
            nextState = Main_SelectFreq();
            break;

        case eMAIN_STATE_SHOW_VOLTAGE:
            nextState = Main_ShowVoltage();
            break;

        case eMAIN_STATE_WORK:
            nextState = Main_Work();
            break;

        case eMAIN_STATE_ERROR:
            nextState = Main_Error();
            break;

        default:
            nextState = eMAIN_STATE_ERROR;
            break;
        }

        // added second buffer for states due to better working of function Main_IsNewState()
        oldState     = currentState;
        currentState = nextState;

        while(timer != 0)
        {
            if(oldTimestamp != timestamp)
            {
                timer--;
                oldTimestamp = timestamp;
            }
        }
        timer = 5;

        DcdcDriver_Perform();
    }

    return 0;
}

//===================================================================================================================//
// Application help functions                                                                                        //
//===================================================================================================================//

static bool Main_IsNewState()
{
    return (currentState != oldState) ? true : false;
}

//===================================================================================================================//
// Main state init functions                                                                                         //
//===================================================================================================================//

static void MainInit_BlinkLed()
{
    for(uint8_t i = 0; i < 4; i++)
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
    DisplayDriver_SetNumber(voltage / 100);
    MainInit_BlinkLed();
    Adc_Perform();

    return eMAIN_STATE_SHOW_VOLTAGE;
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
    Main_states_e state = eMAIN_STATE_SELECT_FREQ;
    uint8_t       selectedFrequency;

    if(Main_IsNewState())
    {
        DisplayDriver_SetMode(eDISPLAY_MODE_ON);
        GPIO_OUT_LED_A_ENABLE();
    }

    selectedFrequency = OutputDriver_GetControlInput();
    DisplayDriver_SetNumber(selectedFrequency);

    // Go to work
    if(Gpio_GetButton(GPIO_BUTTON_A) == eBUTTON_STATUS_PRESSED)
    {
        GPIO_OUT_LED_A_DISABLE();
        state = eMAIN_STATE_WORK;
    }

    // Change output voltage
    if(Gpio_GetButton(GPIO_BUTTON_B) == eBUTTON_STATUS_PRESSED)
    {
        state = eMAIN_STATE_SHOW_VOLTAGE;
    }

    return state;
}

//===================================================================================================================//
// Application functions                                                                                             //
//===================================================================================================================//

/**
 * @brief
 *
 * @return eMAIN_STATE_SELECT_FREQ,
 */
static Main_states_e Main_ShowVoltage()
{
    Main_states_e   state = eMAIN_STATE_SHOW_VOLTAGE;
    static uint16_t timer;

    // Increase again voltage if button was pressed in this state
    if(Main_IsNewState() || (Gpio_GetButton(GPIO_BUTTON_B) == eBUTTON_STATUS_PRESSED))
    {
        timer = 200; // ~2s
        gOutputVoltage += 5000;
        DisplayDriver_SetMode(eDISPLAY_MODE_BLINKING);

        if(gOutputVoltage > DCDC_MAX_OUTPUT_VOLTAGE)
            gOutputVoltage = DCDC_MIN_OUTPUT_VOLTAGE;
        // TODO:: move votlages to system settings
    }

    DisplayDriver_SetNumber(gOutputVoltage / 1000);

    timer--;
    if(timer == 0)
    {
        state = eMAIN_STATE_SELECT_FREQ;
    }

    return state;
}

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
    Main_states_e state = eMAIN_STATE_WORK;
    // Enter state
    if(Main_IsNewState())
    {
        DcdcDriver_SetVoltage(gOutputVoltage);
        DcdcDriver_Enable(true);
        OutputDriver_Init();
        OutputDriver_Enable();
        GPIO_OUT_LED_B_ENABLE();
    }

    // Exit state
    if(Gpio_GetButton(GPIO_BUTTON_A) == eBUTTON_STATUS_PRESSED)
    {
        DcdcDriver_Enable(false);
        OutputDriver_Disable();
        GPIO_OUT_LED_B_DISABLE();
        state = eMAIN_STATE_SELECT_FREQ;
    }

    return state;
}

//===================================================================================================================//
// Application functions                                                                                             //
//===================================================================================================================//

/**
 * @brief
 *
 * @return eMAIN_STATE_ERROR
 */
static Main_states_e Main_Error()
{
    Main_states_e state = eMAIN_STATE_ERROR;
    uint16_t      timer = 2000;

    GPIO_OUT_LED_C_ENABLE();
    DisplayDriver_SetSpecial(eDISPLAY_DIGIT_ERROR);

    return state;
}