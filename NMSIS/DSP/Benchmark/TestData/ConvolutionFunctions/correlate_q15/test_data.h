#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAYA_SIZE_Q15 1024
#define ARRAYB_SIZE_Q15 512

static q15_t correlate_input_q15_A[ARRAYA_SIZE_Q15] = {};
static q15_t correlate_input_q15_B[ARRAYB_SIZE_Q15] = {};
