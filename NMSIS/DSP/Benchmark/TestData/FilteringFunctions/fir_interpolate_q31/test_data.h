#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../fir_interpolate_f32/test_data.h"

#define TEST_LENGTH_SAMPLES 1024
#define NUM_TAPS 32             /* Must be even */
#define L     2                 /* upsample factor */

static q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static q31_t firCoeffs32LP_q31[NUM_TAPS] = {};
