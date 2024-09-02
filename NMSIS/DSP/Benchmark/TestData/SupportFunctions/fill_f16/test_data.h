#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 512

static float16_t f16_fill_value = 55.0f16;

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
