#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAYA_SIZE_Q31 1024
#define ARRAYB_SIZE_Q31 1024

static uint32_t firstIndex = 4;
static uint32_t numPoints = 128;
static q31_t test_conv_input_q31_A[ARRAYA_SIZE_Q31] = {};
static q31_t test_conv_input_q31_B[ARRAYB_SIZE_Q31] = {};
