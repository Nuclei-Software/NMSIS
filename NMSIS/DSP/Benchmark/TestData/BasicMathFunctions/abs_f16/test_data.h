#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)
#define ARRAY_SIZE_F16 1024

static float16_t abs_f16_input[ARRAY_SIZE_F16] = {};
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
