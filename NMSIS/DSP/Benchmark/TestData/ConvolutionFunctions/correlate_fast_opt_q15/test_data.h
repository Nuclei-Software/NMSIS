#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../../../validate.h"

#define ARRAYA_SIZE_Q15 1024
#define ARRAYB_SIZE_Q15 512

static q15_t correlate_input_q15_A[ARRAYA_SIZE_Q15] = {};
static q15_t correlate_input_q15_B[ARRAYB_SIZE_Q15] = {};

static q15_t q15_pScratch1[max(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15) + 2 * min(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15) - 2];
