#pragma once
#include <stdint.h>


#define TEST_LENGTH_SAMPLES 1024 /* 采样点数 */
#define NUM_TAPS 256             /* 2阶IIR滤波的个数 */
#define MU_SIZE 0.2f

static float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static float32_t expectoutput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static float32_t firCoeffs32LP[NUM_TAPS] = {};
