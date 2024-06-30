#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAYA_SIZE_Q7 256
#define ARRAYB_SIZE_Q7 256

static uint32_t firstIndex = 8;
static uint32_t numPoints = 14;
static q7_t test_conv_input_q7_A[ARRAYA_SIZE_Q7] = {};
static q7_t test_conv_input_q7_B[ARRAYB_SIZE_Q7] = {};
