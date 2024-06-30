#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 8

static float16_t mat_inverse_f16_input[M * M] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
