#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAY_SIZE_Q31 128

static q31_t divide_q31_input1[ARRAY_SIZE_Q31] = {};
static q31_t divide_q31_input2[ARRAY_SIZE_Q31] = {};
static int16_t shift_i16[ARRAY_SIZE_Q31] = {};