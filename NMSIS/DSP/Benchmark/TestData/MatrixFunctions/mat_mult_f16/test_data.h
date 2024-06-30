#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 8
#define K 8
#define N 256

float16_t mat_mult_f16_arrayA[M * K];
float16_t mat_mult_f16_arrayB[K * N];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
