#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define CFFTx4SIZE 256

static uint8_t ifftFlag = 0;
static uint8_t doBitReverse = 1;
static q31_t cfft_testinput_q31_50hz_200Hz[2 * CFFTx4SIZE] __attribute__((aligned(16)));
