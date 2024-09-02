#pragma once
#include <stdint.h>


#define ARRAY_SIZE_Q31 1024

static q31_t scaleFract_q31 = -1;
static int8_t shift_q31 = -1;
static q31_t scale_q31_input[ARRAY_SIZE_Q31] = {};
