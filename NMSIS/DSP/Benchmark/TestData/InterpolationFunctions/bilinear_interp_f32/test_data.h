#pragma once
#include <stdint.h>


#define NUM_ROWS 20
#define NUM_COLS 20             /* 2阶IIR滤波的个数 */

static float32_t x_f32_input = 5.12;
static float32_t y_f32_input = 6.24;
static float32_t f32_z_array[NUM_ROWS * NUM_COLS] = {};