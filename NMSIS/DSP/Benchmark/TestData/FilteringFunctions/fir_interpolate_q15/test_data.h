#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 256
#define NUM_TAPS 32             /* Must be even */
#define L     2                 /* upsample factor */

static q15_t testInput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static q15_t firCoeffs32LP_q15[NUM_TAPS] = {};