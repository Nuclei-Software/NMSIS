#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 128

float16_t kullback_leibler_f16_input1[ARRAY_SIZE];
float16_t kullback_leibler_f16_input2[ARRAY_SIZE];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */