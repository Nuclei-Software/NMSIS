#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 512

static float16_t f16_input[ARRAY_SIZE] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */