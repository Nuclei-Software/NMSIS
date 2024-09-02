#pragma once
#include <stdint.h>


#define CFFTSIZE 512

static uint8_t ifftFlag = 0;
static uint8_t doBitReverse = 1;
static float32_t cfft_testinput_f32_50hz_200Hz[2 * CFFTSIZE] __attribute__((aligned(16)));
