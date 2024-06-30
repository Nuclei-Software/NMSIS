#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAY_SIZE_Q7 1024
#define LOWER_BOUND_Q7 -100;
#define HIGHER_BOUND_Q7 100;

static q7_t clip_q7_input[ARRAY_SIZE_Q7] = {};