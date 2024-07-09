#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 16
#define K 16
#define N 256

static q15_t mat_cmplx_mult_q15_arrayA[M * K];
static q15_t mat_cmplx_mult_q15_arrayB[K * N];
