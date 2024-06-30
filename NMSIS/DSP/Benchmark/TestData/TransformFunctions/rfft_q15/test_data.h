#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define RFFTSIZE 512

static uint8_t ifftFlag = 0;
static uint8_t doBitReverse = 1;
static q15_t rfft_testinput_q15_50hz_200Hz[RFFTSIZE];
