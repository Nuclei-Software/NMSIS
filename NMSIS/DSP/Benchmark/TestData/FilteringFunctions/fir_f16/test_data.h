#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../fir_f32/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define TEST_LENGTH_SAMPLES 256
#define NUM_TAPS 32             /* Must be even */

static float16_t testInput_f16_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static float16_t firCoeffs16LP[NUM_TAPS] = {};

#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
