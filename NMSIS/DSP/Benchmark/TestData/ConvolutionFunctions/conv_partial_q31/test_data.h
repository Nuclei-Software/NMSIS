#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAYA_SIZE_Q31 256
#define ARRAYB_SIZE_Q31 256

static uint32_t firstIndex = 2;
static uint32_t numPoints = 100;
static q31_t test_conv_input_q31_A[ARRAYA_SIZE_Q31] = {};
static q31_t test_conv_input_q31_B[ARRAYB_SIZE_Q31] = {};
