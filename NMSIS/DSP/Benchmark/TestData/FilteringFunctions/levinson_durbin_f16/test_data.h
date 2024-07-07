#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define COEFSIZE  256

static float16_t phi[COEFSIZE + 1] = {};
static float16_t autoRegreCoef[COEFSIZE] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
