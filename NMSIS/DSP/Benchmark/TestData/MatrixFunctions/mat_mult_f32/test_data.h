#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 16
#define K 16
#define N 128

float32_t mat_mult_f32_arrayA[M * K];
float32_t mat_mult_f32_arrayB[K * N];
