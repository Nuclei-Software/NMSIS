#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 32
#define N 16
static float16_t mat_sub_f16_arrayA[M * N];
static float16_t mat_sub_f16_arrayB[M * N];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
