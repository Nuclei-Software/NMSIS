#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 128
#define K 8
#define N 8

float32_t mat_mult_f32_arrayA[M * K];
float32_t mat_mult_f32_arrayB[K * N];
