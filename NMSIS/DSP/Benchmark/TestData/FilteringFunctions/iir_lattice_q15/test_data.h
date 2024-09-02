#pragma once
#include <stdint.h>

#include "../iir_lattice_f32/test_data.h"

#define TEST_LENGTH_SAMPLES 1024 /* 采样点数 */
#define numStages 9             /* 2阶IIR滤波的个数 */

static q15_t testInput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {};

static q15_t IIRCoeffs32LP_K_q15[numStages] = {};

static q15_t IIRCoeffs32LP_V_q15[numStages + 1] = {};
