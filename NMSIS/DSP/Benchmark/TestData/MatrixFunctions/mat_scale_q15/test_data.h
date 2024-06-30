#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 16
#define N 16
#define SCALE 32
#define RIGHT_SHIFT 2

q15_t mat_scale_q15_array[M * N];
