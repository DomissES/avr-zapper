/**
 * @file my_assert.h
 * @addtogroup Level_0_Utilities
 *
 * @brief Header file for custom assertion handling.
 *
 * This file provides a macro for assertions that can be enabled or disabled
 * based on the USE_ASSERT flag. If assertions are enabled, the ASSERT macro
 * will check a condition and call the assert_Fail function if the condition is false.
 *
 * @author domis
 * @date 07.03.2025
 *
 */

#ifndef MY_ASSERT_H_
#define MY_ASSERT_H_

#include "stdint.h"

#ifdef USE_ASSERT

__weak void assert_Fail(uint8_t *file, uint16_t line)
{
    (void)file; // Suppress unused parameter warning
    (void)line;

    // This function can be overridden by the user to handle assertion failures
    // For example, it can log the error or enter an infinite loop
    while(1)
        ;
}

    #define ASSERT(condition)                                                                                          \
        do                                                                                                             \
        {                                                                                                              \
            if(!(condition))                                                                                           \
            {                                                                                                          \
                assert_Fail((uint8_t *)__FILE__, __LINE__);                                                            \
            }                                                                                                          \
        } while(0);

#else
    #define ASSERT(condition) (void)0

#endif // USE_ASSERT

#endif // MY_ASSERT_H_