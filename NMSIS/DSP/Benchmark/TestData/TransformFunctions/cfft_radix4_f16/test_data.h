#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define CFFTx4SIZE 256

static uint8_t ifftFlag = 0;
static uint8_t doBitReverse = 1;
static float16_t cfft_testinput_f16_50hz_200Hz[2 * CFFTx4SIZE] __attribute__((aligned(16)));

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
