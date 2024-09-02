#pragma once
#include <stdint.h>

#include "../fir_f32/test_data.h"

#define TEST_LENGTH_SAMPLES_Q7 1024
#define NUM_TAPS_Q7 32             /* Must be even */

static q7_t testInput_q7_50Hz_200Hz[TEST_LENGTH_SAMPLES_Q7] = {};

static q7_t firCoeffLP_q7[NUM_TAPS_Q7] = {};
