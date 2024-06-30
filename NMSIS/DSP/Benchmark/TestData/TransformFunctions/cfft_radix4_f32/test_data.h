#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define CFFTx4SIZE 256

static uint8_t ifftFlag = 0;
static uint8_t doBitReverse = 1;
static float32_t cfft_testinput_f32_50hz_200Hz[2 * CFFTx4SIZE] __attribute__((aligned(16)));
