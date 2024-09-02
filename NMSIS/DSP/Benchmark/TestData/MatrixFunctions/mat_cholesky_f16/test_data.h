#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define M 8

float16_t f16_input_array[M * M] = {};
float16_t f16_posi_array[M * M] = {};
float16_t f16_dot_array[M * M] = {};
float16_t f16_tmp_array[M * M] = {0};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
