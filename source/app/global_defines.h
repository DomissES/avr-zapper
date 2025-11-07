/**
 * @file global_defines.h
 * @addtogroup Level_0_Config
 *
 * @brief Header file which contains all defines for this project.
 *
 * @author domis
 * @date 29.10.2025
 */

// global target includes
#include <stdbool.h>
#include <stdint.h>

// Compiler specific attributes
#define __weak          __attribute__((weak))
#define __packed        __attribute__((packed))
#define __always_inline __attribute__((always_inline)) inline
#define __noinline      __attribute__((noinline))
#define __unused        __attribute__((unused))

// Compiler specific keywords
#define __memx

// String placeholders
#define PSTR(s)                     (const __memx char *)(s)

// Interrupt related
#define ENABLE_GLOBAL_INTERRUPTS()  sei()
#define DISABLE_GLOBAL_INTERRUPTS() cli()