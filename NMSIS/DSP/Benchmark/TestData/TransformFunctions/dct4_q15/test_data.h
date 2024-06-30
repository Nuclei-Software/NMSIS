#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define DCT4SIZE 64

static q15_t normalize = 0x1000;

static q15_t q15_state[DCT4SIZE] = {0};
static q15_t dct4_testinput_q15_50hz_200Hz[2 * DCT4SIZE];
