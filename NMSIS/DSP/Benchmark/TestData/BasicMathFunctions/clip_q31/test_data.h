#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAY_SIZE_Q31 256
#define LOWER_BOUND_Q31 -214712319;
#define HIGHER_BOUND_Q31 214712319;

static q31_t clip_q31_input[ARRAY_SIZE_Q31] = {};