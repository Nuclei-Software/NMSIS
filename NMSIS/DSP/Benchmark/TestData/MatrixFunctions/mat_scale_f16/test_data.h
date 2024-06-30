#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 32
#define N 16
#define SCALE (float16_t)80.73f

static float16_t mat_scale_f16_array[M * N];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
