#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 1024
#define NUM_TAPS 256             /* Must be even */
#define M     64                 /* decimation factor */

static q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static q31_t firCoeffLP_q31[NUM_TAPS] = {};
