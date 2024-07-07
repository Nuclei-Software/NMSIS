#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../../../validate.h"

#define ARRAYA_SIZE_Q7 1024
#define ARRAYB_SIZE_Q7 512

static q7_t correlate_input_q7_A[ARRAYA_SIZE_Q7] = {};
static q7_t correlate_input_q7_B[ARRAYB_SIZE_Q7] = {};

static q15_t q15_pScratch1[max(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7) + 2 * min(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7) - 2];
static q15_t q15_pScratch2[min(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7)];
