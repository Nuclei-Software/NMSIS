#pragma once
#include <stdint.h>


#define TEST_LENGTH_SAMPLES 1024
#define NUM_TAPS 32             /* Must be even */

static q15_t testInput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static q15_t firCoeffLP_q15[NUM_TAPS] = {};
