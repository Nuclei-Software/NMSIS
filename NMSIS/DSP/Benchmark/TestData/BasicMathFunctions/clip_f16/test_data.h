#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE_F16 1024
#define LOWER_BOUND_F16 (float16_t)-4230.37;
#define HIGHER_BOUND_F16 (float16_t)4230.45;

static float16_t clip_f16_input[ARRAY_SIZE_F16] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED)*/
