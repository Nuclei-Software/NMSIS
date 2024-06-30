#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define M 8

static float32_t f32_input_array[M * M] = {};
static float32_t f32_posi_array[M * M] = {};
static float32_t f32_dot_array[M * M] = {};
static float32_t f32_tmp_array[M * M] = {0};
