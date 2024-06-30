#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define DCT4SIZE 64

static q31_t normalize = 0x10000000;

static q31_t q31_state[DCT4SIZE] = {0};
static q31_t dct4_testinput_q31_50hz_200Hz[2 * DCT4SIZE];
