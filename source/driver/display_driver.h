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


const uint8_t DISPLAY_DRIVER_7SEGMENT_MAP[16] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001  // F
};

#endif // DISPLAY_DRIVER_H_