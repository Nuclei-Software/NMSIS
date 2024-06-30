#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 256
#define NUM_TAPS 32             /* Must be even */
#define MAXDelay 29             /* Must be even */

static q7_t testInput_q7_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static int32_t pTapDelay[NUM_TAPS] = {};
static q7_t pTapDelayScratch_q7[TEST_LENGTH_SAMPLES];
static q7_t firCoeffLP_q7[NUM_TAPS] = {};
