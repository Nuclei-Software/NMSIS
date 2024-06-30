#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 32
#define N 16
float16_t mat_add_f16_arrayA[M * N];
float16_t mat_add_f16_arrayB[M * N];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
