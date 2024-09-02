#pragma once
#include <stdint.h>


#define TEST_LENGTH_SAMPLES 1024
#define NUM_TAPS 32             /* Must be even */

static q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static q31_t firCoeffLP_q31[NUM_TAPS] = {};
