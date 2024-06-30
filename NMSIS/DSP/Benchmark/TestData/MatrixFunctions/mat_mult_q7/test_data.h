#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 128
#define K 32
#define N 32

q7_t mat_mult_q7_arrayA[M * K];
q7_t mat_mult_q7_arrayB[K * N];
