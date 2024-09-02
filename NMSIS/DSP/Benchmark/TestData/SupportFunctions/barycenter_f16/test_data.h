#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define VEC_NUM 8
#define DIMENSION 512

float16_t f16_barycenter_array[VEC_NUM * DIMENSION] = {};
float16_t f16_barycenter_weights_array[VEC_NUM];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
