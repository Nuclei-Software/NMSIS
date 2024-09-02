#pragma once
#include <stdint.h>

#include "../fir_sparse_f32/test_data.h"

#define TEST_LENGTH_SAMPLES_Q15 1024
#define NUM_TAPS_Q15 32             /* Must be even */
#define MAXDelayQ15 29             /* Must be even */

static q15_t testInput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES_Q15] = {};
static q15_t pTapDelayScratch_q15[TEST_LENGTH_SAMPLES_Q15];
static q15_t firCoeffLP_q15[NUM_TAPS_Q15] = {};
