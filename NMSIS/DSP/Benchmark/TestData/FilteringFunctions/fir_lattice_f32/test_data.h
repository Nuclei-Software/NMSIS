#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 256
#define NUM_TAPS 32             /* Must be even */

static float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};
static float32_t firCoeffs32LP[NUM_TAPS] = {};