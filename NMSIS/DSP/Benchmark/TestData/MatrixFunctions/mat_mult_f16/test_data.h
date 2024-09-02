#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 16
#define K 16
#define N 256

float16_t mat_mult_f16_arrayA[M * K];
float16_t mat_mult_f16_arrayB[K * N];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
