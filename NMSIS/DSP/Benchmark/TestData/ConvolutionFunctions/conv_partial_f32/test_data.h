#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAYA_SIZE_F32 1024
#define ARRAYB_SIZE_F32 1024

static uint32_t firstIndex = 4;
static uint32_t numPoints = 128;
static float32_t test_conv_input_f32_A[ARRAYA_SIZE_F32] = {};
static float32_t test_conv_input_f32_B[ARRAYB_SIZE_F32] = {};
