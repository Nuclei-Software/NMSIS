#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAY_SIZE_Q7 512

static q7_t scaleFract_q7 = -2;
static int8_t shift_q7 = 9;
static q7_t scale_q7_input[ARRAY_SIZE_Q7] = {};
