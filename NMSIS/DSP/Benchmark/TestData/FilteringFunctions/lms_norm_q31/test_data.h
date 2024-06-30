#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../lms_f32/test_data.h"

#define TEST_LENGTH_SAMPLES 320 /* 采样点数 */
#define NUM_TAPS 32             /* 2阶IIR滤波的个数 */
#define MU_SIZE_Q31 215

static q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static q31_t expectoutput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static q31_t firCoeffs32LP_q31[NUM_TAPS] = {};