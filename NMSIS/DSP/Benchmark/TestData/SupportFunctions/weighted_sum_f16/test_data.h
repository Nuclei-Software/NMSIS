#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define WEIGHT_NUM 512

static float16_t f16_weighted_sum_array[WEIGHT_NUM] = {};
static float16_t f16_weighted_array[WEIGHT_NUM] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
