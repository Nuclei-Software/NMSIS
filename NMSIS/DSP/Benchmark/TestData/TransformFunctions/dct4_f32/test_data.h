#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define DCT4SIZE 64

static float32_t normalize = 0.125;

static float32_t f32_state[DCT4SIZE] = {0};
static float32_t dct4_testinput_f32_50hz_200Hz[2 * DCT4SIZE];
