#pragma once
#include <stdint.h>

#include "../biquad_cascade_df2T_f32/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)
#define TEST_LENGTH_SAMPLES_F16 1024
#define numStagesF16 2             /* 2阶IIR滤波的个数 */

static float16_t testInput_f16_50Hz_200Hz[TEST_LENGTH_SAMPLES_F16] = {};

static float16_t IIRCoeffsF16LP[5 * numStagesF16] = {};

#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
