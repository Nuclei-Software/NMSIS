#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 256
#define K 16
#define N 16

static float16_t mat_cmplx_mult_f16_arrayA[M * K];
static float16_t mat_cmplx_mult_f16_arrayB[K * N];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED)*/
