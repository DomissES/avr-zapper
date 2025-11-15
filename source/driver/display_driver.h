/**
 * @file display_driver.h
 * @addtogroup Level_2_Driver
 *
 * @brief Header file for Display driver
 *
 * Display is in 2nd layer, although it hardly uses any hal underneath it.
 *
 * @author domis
 * @date 07.11.2025
 */

#ifndef DISPLAY_DRIVER_H_
#define DISPLAY_DRIVER_H_

// File specific includes
#include "global_defines.h"
#include "system_settings.h"

// Target specific includes


//===================================================================================================================//
// Public macro defines                                                                                              //
//===================================================================================================================//

//===================================================================================================================//
// Public definitions                                                                                                //
//===================================================================================================================//

typedef enum{
    eDISPLAY_MODE_OFF,
    eDISPLAY_MODE_BLINKING,
    eDISPLAY_MODE_ON
}DisplayDriver_Mode_e;
typedef enum {
    eDISPLAY_DIGIT_ERROR,
    eDISPLAY_DIGIT_BATTERY,
    eDISPLAY_DIGIT_BATTERY_LOW,
    eDISPLAY_DIGIT_MAX_VALUES
}DisplayDriver_SpecialDigit_e;
//===================================================================================================================//
// Public variables                                                                                                  //
//===================================================================================================================//

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

void DisplayDriver_PerformMultiplex();

void DisplayDriver_Init();

void DisplayDriver_SetMode(DisplayDriver_Mode_e mode);

void DisplayDriver_SetNumber(uint8_t number);

void DisplayDriver_SetSpecial(DisplayDriver_SpecialDigit_e digit);


#endif // DISPLAY_DRIVER_H_