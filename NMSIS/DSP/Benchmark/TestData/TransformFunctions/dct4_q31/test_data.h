#pragma once
#include <stdint.h>


#define DCT4SIZE 128

#define NORMALIZE 0x10000000

static q31_t q31_state[DCT4SIZE] = {0};
static q31_t dct4_testinput_q31_50hz_200Hz[DCT4SIZE];
