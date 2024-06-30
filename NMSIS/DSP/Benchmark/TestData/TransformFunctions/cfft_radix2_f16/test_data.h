#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define CFFTSIZE 512

static uint8_t ifftFlag = 0;
static uint8_t doBitReverse = 1;
static float16_t cfft_testinput_f16_50hz_200Hz[2 * CFFTSIZE] __attribute__((aligned(16)));

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
