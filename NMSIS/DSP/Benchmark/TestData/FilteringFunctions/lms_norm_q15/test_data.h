#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 1024 /* 采样点数 */
#define NUM_TAPS 256             /* 2阶IIR滤波的个数 */
#define MU_SIZE_Q15 8

static q15_t testInput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static q15_t expectoutput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static q15_t firCoeffs32LP_q15[NUM_TAPS] = {};
