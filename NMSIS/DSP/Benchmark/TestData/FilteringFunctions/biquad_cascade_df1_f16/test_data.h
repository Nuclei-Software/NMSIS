#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../biquad_cascade_df1_f32/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define TEST_LENGTH_SAMPLES 1024
#define numStages 4             /* 2阶IIR滤波的个数 */

static float16_t testInput_f16_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static float16_t IIRCoeffsF16LP[5 * numStages] = {};

#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
