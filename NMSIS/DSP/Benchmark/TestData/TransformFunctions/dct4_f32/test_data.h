#pragma once
#include <stdint.h>


#define DCT4SIZE 128

#define NORMALIZE 0.125f

static float32_t f32_state[DCT4SIZE] = {0};
static float32_t dct4_testinput_f32_50hz_200Hz[DCT4SIZE];
