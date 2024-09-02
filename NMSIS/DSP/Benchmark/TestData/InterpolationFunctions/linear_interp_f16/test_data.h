#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 90

static float16_t x1 = 0;
static float16_t xSpacing = (float16_t)123.12;
static float16_t x_f16_input = (float16_t)5.12f;
static float16_t f16_y_array[ARRAY_SIZE] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */