#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES 1024
#define numStages 4             /* 2阶IIR滤波的个数 */

static q15_t testInput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static q15_t IIRCoeffsQ15LP[6 * numStages] = {
    32767,0,32767,32767,32767,-18810,32767,0,32767,32767,28029,-6871,
    32767,0,32767,32767,32767,-18810,32767,0,32767,32767,28029,-6871};
