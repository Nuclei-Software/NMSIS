#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)
#define ARRAY_SIZE_F16 512

static float16_t cosine_distance_f16_input1[ARRAY_SIZE_F16] = {};
static float16_t cosine_distance_f16_input2[ARRAY_SIZE_F16] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
