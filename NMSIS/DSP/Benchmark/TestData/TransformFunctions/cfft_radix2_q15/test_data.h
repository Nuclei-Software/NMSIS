#pragma once
#include <stdint.h>


#define CFFTSIZE 512

static uint8_t ifftFlag = 0;
static uint8_t doBitReverse = 1;
static q15_t cfft_testinput_q15_50hz_200Hz[2 * CFFTSIZE] __attribute__((aligned(16)));
