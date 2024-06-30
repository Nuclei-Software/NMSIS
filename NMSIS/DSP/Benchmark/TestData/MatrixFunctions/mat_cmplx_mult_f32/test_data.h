#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 128
#define K 16
#define N 16

static float32_t mat_cmplx_mult_f32_arrayA[M * K];
static float32_t mat_cmplx_mult_f32_arrayB[K * N];
