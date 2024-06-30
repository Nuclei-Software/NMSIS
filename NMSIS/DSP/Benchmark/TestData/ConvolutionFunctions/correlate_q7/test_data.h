#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../../../validate.h"

#define ARRAYA_SIZE_Q7 257
#define ARRAYB_SIZE_Q7 257

static q7_t correlate_input_q7_A[ARRAYA_SIZE_Q7] = {};
static q7_t correlate_input_q7_B[ARRAYB_SIZE_Q7] = {};
