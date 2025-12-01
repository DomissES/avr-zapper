/**
 * @file output_driver_table.h
 *
 * @brief Header file with table with predefined values for timer 2
 * @author domis
 * @date 20.11.2025
 */

#ifndef OUTPUT_DRIVER_TABLE_H_
#define OUTPUT_DRIVER_TABLE_H_

// File specific includes
#include "global_defines.h"
#include "system_settings.h"

// Target specific includes

#define OUTPUT_DRIVER_TABLE_SIZE                     100
#define OUTPUT_DRIVER_BATTERY_OFFSET_TABLE_SIZE      16
#define OUTPUT_DRIVER_BATTERY_OFFSET_REFERENCE_INDEX 13

/**
 * @brief This table consists ocr and prescaler values for specific frequency.
 * First row is the ocr
 * Second row is the prescaler (3 for 32, 2 for 8, 1 for 1)
 *
 */
uint8_t outputDriver_table[OUTPUT_DRIVER_TABLE_SIZE][2] = {
    {124, 3}, {249, 2}, {165, 2}, {124, 2}, {99, 2},  {82, 2},  {70, 2},  {61, 2},  {55, 2},  {49, 2},
    {44, 2},  {41, 2},  {37, 2},  {35, 2},  {32, 2},  {249, 1}, {234, 1}, {221, 1}, {210, 1}, {199, 1},
    {189, 1}, {181, 1}, {173, 1}, {166, 1}, {159, 1}, {153, 1}, {147, 1}, {142, 1}, {137, 1}, {132, 1},
    {128, 1}, {124, 1}, {120, 1}, {117, 1}, {113, 1}, {110, 1}, {107, 1}, {104, 1}, {102, 1}, {99, 1},
    {97, 1},  {94, 1},  {92, 1},  {90, 1},  {88, 1},  {86, 1},  {84, 1},  {82, 1},  {81, 1},  {79, 1},
    {77, 1},  {76, 1},  {74, 1},  {73, 1},  {72, 1},  {70, 1},  {69, 1},  {68, 1},  {67, 1},  {66, 1},
    {65, 1},  {64, 1},  {62, 1},  {62, 1},  {61, 1},  {60, 1},  {59, 1},  {58, 1},  {57, 1},  {56, 1},
    {55, 1},  {55, 1},  {54, 1},  {53, 1},  {52, 1},  {52, 1},  {51, 1},  {50, 1},  {50, 1},  {49, 1},
    {48, 1},  {48, 1},  {47, 1},  {47, 1},  {46, 1},  {46, 1},  {45, 1},  {44, 1},  {44, 1},  {43, 1},
    {43, 1},  {42, 1},  {42, 1},  {42, 1},  {41, 1},  {41, 1},  {40, 1},  {40, 1},  {39, 1},  {39, 1}};

/**
 * @brief In case of other input voltage, change the overall range of Control input
 *
 */
uint16_t outputDriver_BatteryOffsetTable[] = {
    754, // 3.7 V
    774, // 3.8 V
    796, // 3.9 V
    816, // 4.0 V
    836, // 4.1 V
    858, // 4.2 V
    878, // 4.3 V
    898, // 4.4 V
    920, // 4.5 V
    939, // 4.6 V
    959, // 4.7 V
    980, // 4.8 V
    998, // 4.9 V
    998, // 5.0 V
    998, // 5.1 V
    998, // 5.2 V
};

#endif // OUTPUT_DRIVER_TABLE_H_