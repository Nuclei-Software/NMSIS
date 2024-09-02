#pragma once
#include <stdint.h>

#include "../fir_f32/test_data.h"

#define TEST_LENGTH_SAMPLES_F64 1024
#define NUM_TAPS_F64 32             /* Must be even */

static float64_t testInput_f64_50Hz_200Hz[TEST_LENGTH_SAMPLES_F64] = {};

static float64_t firCoeffs64LP[NUM_TAPS_F64] = {};
