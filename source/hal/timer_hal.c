/**
 * @file timer_hal.c
 * @addtogroup Level_1_HAL
 *
 * @brief
 *
 *  In this system there will be two timers:
 *  Timer 1:
 *      Used for DC-DC key. This timer needs to run at best 250 kHz. Currently it will be 125 kHz.
 *      The mode is phase correct PWM signal. OC1B is used as an PWM output, OC1A is used as a frequency selector.
 *      It is a 16-bit timer so it has more flexibility, alth
 *  Timer 2:
 *      Used for OUT key. It uses PWM signal OC2. Main regulation will be the OCR2, which will set proper frequency.
 *      It is an 8-bit timer.
 *
 * @author domis
 * @date 30.10.2025
 */

// File specific includes
#include "timer_hal.h"

#include "global_defines.h"
#include "my_assert.h"
#include "system_settings.h"

// Target specific includes
#include <avr/interrupt.h>
#include <avr/io.h>

//===================================================================================================================//
// Private macro defines                                                                                              //
//===================================================================================================================//

#define TIMER_1_SFR_INIT()                                                                                             \
    {                                                                                                                  \
        pTimer->SFR.tccrA = &TCCR1A;                                                                                   \
        pTimer->SFR.tccrB = &TCCR1B;                                                                                   \
        pTimer->SFR.ocrA   = &OCR1A;                                                                                    \
        pTimer->SFR.ocrB   = &OCR1B;                                                                                    \
        pTimer->SFR.tcnt  = &TCNT1;                                                                                    \
    }

#define TIMER_2_SFR_INIT()                                                                                             \
    {                                                                                                                  \
        pTimer->SFR.tccr = &TCCR2;                                                                                     \
        pTimer->SFR.ocr  = &OCR2;                                                                                      \
        pTimer->SFR.tcnt = &TCNT2;                                                                                     \
    }

#define TIMER_1_PRESCALER_1    1
#define TIMER_1_PRESCALER_8    2
#define TIMER_1_PRESCALER_64   3
#define TIMER_1_PRESCALER_256  4
#define TIMER_1_PRESCALER_1024 5

#define TIMER_1_USED_PRESCALER TIMER_1_PRESCALER_1

#define TIMER_2_PRESCALER_1    1
#define TIMER_2_PRESCALER_8    2
#define TIMER_2_PRESCALER_32   3
#define TIMER_2_PRESCALER_64   4
#define TIMER_2_PRESCALER_128  5
#define TIMER_2_PRESCALER_256  6
#define TIMER_2_PRESCALER_1024 7

#define TIMER_2_USED_PRESCALER TIMER_2_PRESCALER_32

//===================================================================================================================//
// Timer interrupts definitions                                                                                      //
//===================================================================================================================//


//===================================================================================================================//
// Private definitions                                                                                               //
//===================================================================================================================//

//===================================================================================================================//
// Private variables                                                                                                  //
//===================================================================================================================//

Timer_1_HAL_t hTimer1;
Timer_2_HAL_t hTimer2;
//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Initializes timer 1 or 2
 *
 * Timer 1 is for DC DC key.
 * Timer 2 is for Output key.
 * After initialization the timers are in idle mode. You need first to start the timers in order to run them properly.
 *
 * @param pTimer pointer to timer instance
 */
void TimerHAL_InitTimer_1(Timer_1_HAL_t *pTimer)
{
    ASSERT(pTimer != NULL);

    TIMER_1_SFR_INIT();
    pTimer->instance  = eTIMER_1;
    pTimer->running   = 0;
    pTimer->enableOut = 0;

    // Configure timer as mode 11, PWM phase correct
    // top is OCR1A
    *pTimer->SFR.tccrA = _BV(WGM10) | _BV(WGM11);
    *pTimer->SFR.tccrB = _BV(WGM13);

    // Set frequency of 125 kHz by setting value for OCR A
    *pTimer->SFR.ocrA = 32;
}

void TimerHAL_InitTimer_2(Timer_2_HAL_t *pTimer)
{
    ASSERT(pTimer != NULL);

    TIMER_2_SFR_INIT();
    pTimer->instance  = eTIMER_2;
    pTimer->running   = 0;
    pTimer->enableOut = 0;

    // Configure timer as CTC
    *pTimer->SFR.tccr = _BV(WGM21);

    // Set OCR to defult value
    *pTimer->SFR.ocr = 0x80;

}

/**
 * @brief Starts or stops the timer
 *
 * This functions put the timers into running state or idle state.
 *
 * @param timer index to the timer intance.
 */
void TimerHAL_StopTimer(Timer_index_e timer)
{
    if(timer == eTIMER_1)
        *hTimer1.SFR.tccrB = *hTimer1.SFR.tccrB & 0xF8;
    else
        *hTimer2.SFR.tccr = *hTimer2.SFR.tccr & 0xF8;
}

/**
 * @brief Starts or stops the timer
 *
 * This functions put the timers into running state or idle state.
 *
 * @param timer index to the timer intance.
 */
void TimerHAL_StartTimer(Timer_index_e timer)
{
    if(timer == eTIMER_1)
        *hTimer1.SFR.tccrB = (*hTimer1.SFR.tccrB & 0xF8) | TIMER_1_USED_PRESCALER;
    else
        *hTimer2.SFR.tccr = (*hTimer2.SFR.tccr & 0xF8) | TIMER_2_USED_PRESCALER;
}

/**
 * @brief Changes Output compare value for specific timer
 *
 * Changing OCR value for timer 1 is for changing the duty cycle of PWM.
 * Changing OCR value for timer 2 is for changing the overal period of PWM.
 *
 * @param timer index to the timer instance.
 * @param ocrValue 16-bit value for timer 1, 8-bit value for timer 2
 */
void TimerHAL_SetOCR(Timer_index_e timer, uint16_t ocrValue)
{
    if(timer == eTIMER_1)
    {
        *hTimer1.SFR.ocrB = ocrValue;
    }
    else
    {
        *hTimer2.SFR.ocr = (uint8_t)ocrValue;
    }
}