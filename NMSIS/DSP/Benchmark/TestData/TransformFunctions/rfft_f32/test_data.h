#pragma once
#include <stdint.h>


#define RFFTSIZE 512

static uint8_t ifftFlag = 0;
static uint8_t doBitReverse = 1;
static float32_t rfft_testinput_f32_50hz_200Hz[RFFTSIZE];
