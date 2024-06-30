#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAY_SIZE_Q15 512

static q15_t scaleFract_q15 = -2;
static int8_t shift_q15 = 17;
static q15_t scale_q15_input[ARRAY_SIZE_Q15] = {};
