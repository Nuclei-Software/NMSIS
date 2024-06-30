#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define VEC_NUM 8
#define DIMENSION 256

float32_t f32_barycenter_array[VEC_NUM * DIMENSION] = {};
float32_t f32_barycenter_weights_array[VEC_NUM];
