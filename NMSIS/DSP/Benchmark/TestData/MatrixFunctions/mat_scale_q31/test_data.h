#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 16
#define N 16
#define SCALE 64
#define RIGHT_SHIFT 2

q31_t mat_scale_q31_array[M * N];
