#pragma once
#include <stdint.h>

#include "../fir_sparse_f32/test_data.h"

#define TEST_LENGTH_SAMPLES_Q31 1024
#define NUM_TAPS_Q31 32             /* Must be even */
#define MAXDelayQ31 29             /* Must be even */

static q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES_Q31] = {};
static q31_t pTapDelayScratch_q31[TEST_LENGTH_SAMPLES_Q31];
static q31_t firCoeffLP_q31[NUM_TAPS_Q31] = {};
