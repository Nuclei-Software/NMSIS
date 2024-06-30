#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../biquad_cascade_df1_f32/test_data.h"

#define TEST_LENGTH_SAMPLES 256
#define numStages 2             /* 2阶IIR滤波的个数 */

static float64_t testInput_f64_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static float64_t IIRCoeffs64LP[5 * numStages] = {};
