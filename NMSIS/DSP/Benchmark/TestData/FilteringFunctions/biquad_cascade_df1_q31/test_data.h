#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 1024
#define numStages 4             /* 2阶IIR滤波的个数 */

static q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static q31_t IIRCoeffsQ31LP[5 * numStages] = {};
