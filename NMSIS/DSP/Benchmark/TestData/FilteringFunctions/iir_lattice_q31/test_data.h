#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../iir_lattice_f32/test_data.h"

#define TEST_LENGTH_SAMPLES 1024 /* 采样点数 */
#define numStages 9             /* 2阶IIR滤波的个数 */

static q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static q31_t IIRCoeffs32LP_K_q31[numStages] = {};

static q31_t IIRCoeffs32LP_V_q31[numStages + 1] = {};
