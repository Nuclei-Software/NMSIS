#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 16
#define K 16
#define N 128

q7_t mat_mult_q7_arrayA[M * K];
q7_t mat_mult_q7_arrayB[K * N];
