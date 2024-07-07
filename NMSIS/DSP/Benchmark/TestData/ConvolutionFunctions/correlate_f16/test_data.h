#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAYA_SIZE_F16 1024
#define ARRAYB_SIZE_F16 512

static float16_t correlate_input_f16_A[ARRAYA_SIZE_F16] = {};
static float16_t correlate_input_f16_B[ARRAYB_SIZE_F16] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
