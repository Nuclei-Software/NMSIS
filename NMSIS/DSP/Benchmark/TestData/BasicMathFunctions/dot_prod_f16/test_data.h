#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE_F16 1024

static float16_t dot_prod_f16_input1[ARRAY_SIZE_F16] = {};
static float16_t dot_prod_f16_input2[ARRAY_SIZE_F16] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED)*/