#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define ARRAY_SIZE_F32 256
#define LOWER_BOUND_F32 -4230.37;
#define HIGHER_BOUND_F32 4230.45;

static float32_t clip_f32_input[ARRAY_SIZE_F32] = {};