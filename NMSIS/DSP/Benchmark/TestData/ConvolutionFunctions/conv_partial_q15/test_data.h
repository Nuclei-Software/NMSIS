#pragma once
#include <stdint.h>


#define ARRAYA_SIZE_Q15 1024
#define ARRAYB_SIZE_Q15 1024

static uint32_t firstIndex = 4;
static uint32_t numPoints = 128;
static q15_t test_conv_input_q15_A[ARRAYA_SIZE_Q15] = {};
static q15_t test_conv_input_q15_B[ARRAYB_SIZE_Q15] = {};
