#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 512
#define N 8

static float16_t mat_trans_f16_array[M * N];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
