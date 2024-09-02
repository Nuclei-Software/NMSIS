#pragma once
#include <stdint.h>


#define TEST_LENGTH_SAMPLES 1024
#define numStages 2             /* 2阶IIR滤波的个数 */

static float64_t testInput_f64_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static float64_t IIRCoeffs64LP[5 * numStages] = {};
