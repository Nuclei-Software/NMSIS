#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 256
#define NUM_TAPS 32             /* Must be even */
#define M     32                 /* decimation factor */

static q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static q31_t firCoeffLP_q31[NUM_TAPS] = {};