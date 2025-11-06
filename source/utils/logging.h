/**
 * @file logging.h
 * @addtogroup Level_0_Utilities
 *
 * @brief This file provides logging macros for different log levels.
 *
 * @author domis
 * @date 28.02.2025
 *
 */

#ifndef LOGGING_H_
#define LOGGING_H_

// File specific include

//===================================================================================================================//
// Default macro definitions                                                                                         //
//===================================================================================================================//

#ifndef LOG_PRINTF_FUNC
    #error "Define LOG_PRINTF_FUNC to allow for logging. Example: #define LOG_PRINTF_FUNC(...) printf(__VA_ARGS__);"
#endif

#ifndef LOG_TIME_FUNC
    #define LOG_TIME_FUNC() 0
#endif

#ifndef LOG_TIME_FORMAT
    #define LOG_TIME_FORMAT " %d "
#endif

#ifndef LOG_LEVEL
    #warning "LOG_LEVEL not defined, defaults to LOG_LEVEL_DISABLE"
#endif

// Log levels
#define LOG_LEVEL_DISABLE 0
#define LOG_LEVEL_FATAL   1
#define LOG_LEVEL_ERROR   2
#define LOG_LEVEL_WARN    3
#define LOG_LEVEL_DEBUG   4

//===================================================================================================================//
// Logging macros                                                                                                    //
//===================================================================================================================//

#if LOG_SHOW_FUNCTION_LINES == 1

    #if LOG_LEVEL >= LOG_LEVEL_DEBUG
        #define LOG_DEBUG(msg, args...)                                                                                \
            LOG_PRINTF_FUNC("[" LOG_TIME_FORMAT "DEBUG]\t" msg "\r\n", LOG_TIME_FUNC(), ##args, __FUNCTION__, __LINE__);
    #else
        #define LOG_DEBUG(...)                                                                                         \
            {                                                                                                          \
            }
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_WARN
        #define LOG_WARN(msg, args...)                                                                                 \
            LOG_PRINTF_FUNC("[" LOG_TIME_FORMAT "WARN]\t" msg "\r\n", LOG_TIME_FUNC(), ##args, __FUNCTION__, __LINE__);
    #else
        #define LOG_WARN(...)                                                                                          \
            {                                                                                                          \
            }
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_ERROR
        #define LOG_ERROR(msg, args...)                                                                                \
            LOG_PRINTF_FUNC("[" LOG_TIME_FORMAT "ERROR]\t" msg "\r\n", LOG_TIME_FUNC(), ##args, __FUNCTION__, __LINE__);
    #else
        #define LOG_ERROR(...)                                                                                         \
            {                                                                                                          \
            }
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_FATAL
        #define LOG_FATAL(msg, args...)                                                                                \
            LOG_PRINTF_FUNC("[" LOG_TIME_FORMAT "FATAL]\t" msg "\r\n", LOG_TIME_FUNC(), ##args, __FUNCTION__, __LINE__);
    #else
        #define LOG_FATAL(...)                                                                                         \
            {                                                                                                          \
            }
    #endif

#else
    #if LOG_LEVEL >= LOG_LEVEL_DEBUG
        #define LOG_DEBUG(msg, args...)                                                                                \
            LOG_PRINTF_FUNC("[" LOG_TIME_FORMAT "DEBUG]\t" msg "\r\n", LOG_TIME_FUNC(), ##args);
    #else
        #define LOG_DEBUG(...)                                                                                         \
            {                                                                                                          \
            }
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_WARN
        #define LOG_WARN(msg, args...)                                                                                 \
            LOG_PRINTF_FUNC("[" LOG_TIME_FORMAT "WARN]\t" msg "\r\n", LOG_TIME_FUNC(), ##args);
    #else
        #define LOG_WARN(...)                                                                                          \
            {                                                                                                          \
            }
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_ERROR
        #define LOG_ERROR(msg, args...)                                                                                \
            LOG_PRINTF_FUNC("[" LOG_TIME_FORMAT "ERROR]\t" msg "\r\n", LOG_TIME_FUNC(), ##args);
    #else
        #define LOG_ERROR(...)                                                                                         \
            {                                                                                                          \
            }
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_FATAL
        #define LOG_FATAL(msg, args...)                                                                                \
            LOG_PRINTF_FUNC("[" LOG_TIME_FORMAT "FATAL]\t" msg "\r\n", LOG_TIME_FUNC(), ##args);
    #else
        #define LOG_FATAL(...)                                                                                         \
            {                                                                                                          \
            }
    #endif
#endif // LOG_SHOW_FUNCTION_LINES == 0

#endif // LOGGING_H_