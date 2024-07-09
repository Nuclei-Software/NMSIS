#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 16
#define K 16
#define N 128

static q31_t mat_cmplx_mult_q31_arrayA[M * K];
static q31_t mat_cmplx_mult_q31_arrayB[K * N];
