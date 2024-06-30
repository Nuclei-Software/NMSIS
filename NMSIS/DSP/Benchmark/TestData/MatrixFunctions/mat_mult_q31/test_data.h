#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 64
#define K 128
#define N 8

static q31_t mat_mult_q31_arrayA[M * K];
static q31_t mat_mult_q31_arrayB[K * N];
