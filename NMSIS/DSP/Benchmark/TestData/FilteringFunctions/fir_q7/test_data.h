#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 256
#define NUM_TAPS 32             /* Must be even */

static q7_t testInput_q7_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static q7_t firCoeffLP_q7[NUM_TAPS] = {};