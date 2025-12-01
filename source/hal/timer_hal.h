/**
 * @file timer_hal.h
 * @addtogroup Level_1_HAL
 *
 * @brief Header file for timer HAL
 *
 * In this system there will be three timers:
 *  Timer 0:
 *      Used as a system timer. Mainly it will be used for driving some synchronous tasks, such as display
 *      multiplexing, software timers by increasing it's timestamp, etc. It is an 8 bit timer.
 *  Timer 1:
 *      Used for DC-DC key. It uses PWM signal OC1B. Main regulation will be the duty cycle.
 *      It is an 16-bit timer.
 *  Timer 2:
 *      Used for Output key. It uses PWM signal OC2. Possible regulators will be the frequency of the pwm signal.
 *      It is a 8-bit timer.
 *
 * @author domis
 * @date 17.10.2025
 */

#ifndef TIMER_HAL_H
#define TIMER_HAL_H

// File specific includes
#include "global_defines.h"
// Target specific includes
#include <avr/io.h>

//===================================================================================================================//
// Public macro defines                                                                                              //
//===================================================================================================================//

//===================================================================================================================//
// Public definitions                                                                                                //
//===================================================================================================================//

typedef enum
{
    eTIMER_NONE = 0,
    eTIMER_0    = 1,
    eTIMER_1    = 2,
    eTIMER_2    = 3
} Timer_index_e;

typedef struct
{
    // Not all registers are here listed! Only used for this project
    struct Timer_0_SFR
    {
        volatile uint8_t *tccr;
        volatile uint8_t *tcnt;
    } SFR;
    Timer_index_e instance : 2;
    bool          running  : 1;

} Timer_0_HAL_t;

typedef struct
{
    // Not all registers are here listed! Only used for this project
    struct Timer_1_SFR
    {
        volatile uint8_t  *tccrA;
        volatile uint8_t  *tccrB;
        volatile uint16_t *ocrA;
        volatile uint16_t *ocrB;
        volatile uint16_t *tcnt;
    } SFR;
    Timer_index_e instance  : 2;
    bool          running   : 1;
    bool          enableOut : 1;
} Timer_1_HAL_t;

typedef struct
{
    // Not all registers are here listed! Only used for this project
    struct Timer_2_SFR
    {
        volatile uint8_t *tccr;
        volatile uint8_t *ocr;
        volatile uint8_t *tcnt;
    } SFR;
    Timer_index_e instance      : 2;
    bool          running       : 1;
    bool          enableOut     : 1;
    uint8_t       usedPrescaler : 3;
} Timer_2_HAL_t;

//===================================================================================================================//
// Public variables                                                                                                  //
//===================================================================================================================//

extern Timer_0_HAL_t hTimer0;
extern Timer_1_HAL_t hTimer1;
extern Timer_2_HAL_t hTimer2;

extern volatile uint32_t timestamp;

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

/**
 * @brief Enables OCR output for Timer 1
 * The OCR is toggling each time of timer overflow
 *
 */
#define TIMER_HAL_ENABLE_OCR1()  TCCR1A |= _BV(COM1B1)

/**
 * @brief Disables OCR output for Timer 1
 * The OCR is toggling each time of timer overflow
 *
 */
#define TIMER_HAL_DISABLE_OCR1() TCCR1A &= ~_BV(COM1A0)

/**
 * @brief Enables OCR output for Timer 2
 */
#define TIMER_HAL_ENABLE_OCR2()  TCCR2 |= _BV(COM20)

/**
 * @brief Disables OCR output for Timer 2
 */
#define TIMER_HAL_DISABLE_OCR2() TCCR2 &= ~(_BV(COM21) & _BV(COM21))

/**
 * @brief Callback function for timer 0 overlow.
 *
 * Use this function for executing synchronous tasks.
 *
 */
void TimerHAL_Timer0_OverflowCallback();

/**
 * @brief Initializes timer 0, 1 or 2
 *
 * Timer 0 acts as a system timer.
 * Timer 1 is for DC DC key.
 * Timer 2 is for Output key.
 * After initialization the timers are in idle mode. You need first to start the timers in order to run them properly.
 *
 * @param pTimer pointer to timer instance
 */
void TimerHAL_InitTimer_0(Timer_0_HAL_t *pTimer);
void TimerHAL_InitTimer_1(Timer_1_HAL_t *pTimer);
void TimerHAL_InitTimer_2(Timer_2_HAL_t *pTimer);

/**
 * @brief Starts or stops the timer
 *
 * This functions put the timers into running state or idle state.
 *
 * @param timer index to the timer intance.
 */
void TimerHAL_StopTimer(Timer_index_e timer);
void TimerHAL_StartTimer(Timer_index_e timer);

/**
 * @brief Checks if the timer is currently running.
 *
 * @param timer timer index to the timer
 * @return true if is running, false otherwise.
 */
bool TimerHAL_IsTimerEnabled(Timer_index_e timer);

/**
 * @brief Changes Output compare value for specific timer
 *
 * Changing OCR value for timer 1 is for changing the duty cycle of PWM.
 * Changing OCR value for timer 2 is for changing the overal period of PWM.
 *
 * @param timer index to the timer instance.
 * @param ocrValue 16-bit value for timer 1, 8-bit value for timer 2
 */
void TimerHAL_SetOCR(Timer_index_e timer, uint16_t ocrValue);

/**
 * @brief Sets proper prescaler for given timer
 *
 * Currently only timer 2 is supported.
 *
 * @param timer index to the timer instance.
 * @param prescaler prescaler to be set. Possible values are 0..7.
 */
void TimerHAL_SetPrescaler(Timer_index_e timer, uint8_t prescaler);

#endif // TIMER_HAL_H