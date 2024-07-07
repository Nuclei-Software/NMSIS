#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 1024
#define numStages 2             /* 2阶IIR滤波的个数 */

static float32_t testInput_f32_50Hz_200Hz[2 * TEST_LENGTH_SAMPLES] = {};

static float32_t IIRCoeffs32LP[5 * numStages] = {
    1.0f, 2.0f, 1.0f, 1.11302985416334787593939381622476503253f,  -0.574061915083954765748330828500911593437f, 1.0f,
    2.0f,  1.0f,   0.855397932775170177777113167394418269396f, - 0.209715357756554754420363906319835223258f};
