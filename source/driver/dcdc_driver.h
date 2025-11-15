/**
 * @file dcdc_driver.h
 * @addtogroup Level_2_Driver
 * @brief Header file for DC_DC step up converter.
 *
 * @author domis
 * @date 07.11.2025
 */

#ifndef DCDC_DRIVER_H_
#define DCDC_DRIVER_H_

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

void DcdcDriver_ProcessMeasurement(uint16_t measurement);

bool DcdcDriver_Init();

void DcdcDriver_Enable(bool on);

void DcdcDriver_SetVoltage(uint16_t voltageLevel);

uint16_t DcdcDriver_GetVoltage();

void DcdcDriver_Perform();

#endif // DCDC_DRIVER_H_