#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 256
#define NUM_TAPS 32             /* Must be even */
#define MAXDelay 29             /* Must be even */

static q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static int32_t pTapDelay[NUM_TAPS] = {0};
static q31_t pTapDelayScratch_q31[TEST_LENGTH_SAMPLES];
static q31_t firCoeffLP_q31[NUM_TAPS] = {};
