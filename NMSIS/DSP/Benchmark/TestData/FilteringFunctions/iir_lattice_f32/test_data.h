#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 320 /* 采样点数 */
#define numStages 9             /* 2阶IIR滤波的个数 */

static float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static float32_t IIRCoeffs32LP_K[numStages] = {
    -0.9490513802f, 0.960866034f, -0.950527668f, 0.9305356741f, -0.8808346987f, 0.7298035026f,
    -0.3796664774f, 0.07335939258f, 0.2367134901f};

static float32_t IIRCoeffs32LP_V[numStages + 1] = {
    0.0004670530325f, 0.002966811415f, 0.006142409053f, 0.006682873704f, 0.004372655414f, 0.00177692459f,
    0.0004465713282f, 6.86832791e-05f, 5.115610293e-06f, 0.001267434f};