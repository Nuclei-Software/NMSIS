#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 8
#define N 8

static float16_t mat_sl_f16_input[M * N];
static float16_t mat_lt[M * M];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
