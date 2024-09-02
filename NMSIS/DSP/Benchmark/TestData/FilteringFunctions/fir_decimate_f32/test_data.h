#pragma once
#include <stdint.h>


#define TEST_LENGTH_SAMPLES 1024
#define NUM_TAPS 256             /* Must be even */
#define M    64                 /* decimation factor */

static float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static float32_t firCoeffs32LP[NUM_TAPS] = {};
