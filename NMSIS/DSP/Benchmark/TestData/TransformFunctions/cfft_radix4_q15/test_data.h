#pragma once
#include <stdint.h>


#define CFFTx4SIZE 256

static uint8_t ifftFlag = 0;
static uint8_t doBitReverse = 1;
static q15_t cfft_testinput_q15_50hz_200Hz[2 * CFFTx4SIZE] __attribute__((aligned(16)));
