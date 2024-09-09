#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../park_f32/test_data.h"

#define ARRAY_SIZE_Q31 256

static q31_t Ia_q31[ARRAY_SIZE_Q31] = {};
static q31_t Ib_q31[ARRAY_SIZE_Q31] = {};
static q31_t pIalpha_q31[ARRAY_SIZE_Q31] = {};
static q31_t pIbeta_q31[ARRAY_SIZE_Q31] = {};
static q31_t sinVal_q31[ARRAY_SIZE_Q31] = {};
static q31_t cosVal_q31[ARRAY_SIZE_Q31] = {};
