#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define NUM_ROWS 20
#define NUM_COLS 20             /* 2阶IIR滤波的个数 */


static float16_t x_f16_input = (float16_t)5.12;
static float16_t y_f16_input = (float16_t)6.24;
static float16_t f16_z_array[NUM_ROWS * NUM_COLS] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */