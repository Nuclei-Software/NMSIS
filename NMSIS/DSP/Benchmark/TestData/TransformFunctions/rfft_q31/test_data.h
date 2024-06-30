#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define RFFTSIZE 512

uint8_t ifftFlag = 0;
uint8_t doBitReverse = 1;
q31_t rfft_testinput_q31_50hz_200Hz[RFFTSIZE];