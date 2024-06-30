#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAYA_SIZE_Q15 256
#define ARRAYB_SIZE_Q15 256

static uint32_t firstIndex = 4;
static uint32_t numPoints = 100;
static q15_t test_conv_input_q15_A[ARRAYA_SIZE_Q15] = {};
static q15_t test_conv_input_q15_B[ARRAYB_SIZE_Q15] = {};
