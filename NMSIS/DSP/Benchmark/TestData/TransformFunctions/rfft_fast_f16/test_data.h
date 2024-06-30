#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define RFFTSIZE 512

static uint8_t ifftFlag = 0;
static float16_t rfft_testinput_f16_50hz_200Hz_fast[RFFTSIZE];
