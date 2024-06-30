#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 256
#define NUM_TAPS 32             /* Must be even */

static float64_t testInput_f64_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static float64_t firCoeffs64LP[NUM_TAPS] = {};