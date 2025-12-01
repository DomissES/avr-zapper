/**
 * @file output_driver.h
 * @addtogroup Level_2_Driver
 *
 * @brief Header file for output frequency driver
 *
 * @author domis
 * @date 16.11.2025
 */

#ifndef OUTPUT_DRIVER_H_
#define OUTPUT_DRIVER_H_

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

//===================================================================================================================//
// Public variables                                                                                                  //
//===================================================================================================================//

//===================================================================================================================//
// Public functions                                                                                                  //
//===================================================================================================================//

void OutputDriver_PerformControlInput(uint16_t measurement);

void OutputDriver_Init();

void OutputDriver_Enable();

void OutputDriver_Disable();

void OutputDriver_SetFrequency(uint16_t frequency);

uint16_t OutputDriver_GetControlInput();

#endif // OUTPUT_DRIVER_H_