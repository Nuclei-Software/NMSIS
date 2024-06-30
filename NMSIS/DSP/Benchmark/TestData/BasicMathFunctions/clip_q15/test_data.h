#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAY_SIZE_Q15 512
#define LOWER_BOUND_Q15 -2638;
#define HIGHER_BOUND_Q15 1000;

static q15_t clip_q15_input[ARRAY_SIZE_Q15] = {};