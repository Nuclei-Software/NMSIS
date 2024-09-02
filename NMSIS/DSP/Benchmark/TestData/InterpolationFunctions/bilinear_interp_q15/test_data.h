#pragma once
#include <stdint.h>


#define NUM_ROWS 20
#define NUM_COLS 20             /* 2阶IIR滤波的个数 */

static q31_t x_q31_input = 5305794;
static q31_t y_q31_input = 6417285;
static q15_t q15_z_array[NUM_ROWS * NUM_COLS] = {};
