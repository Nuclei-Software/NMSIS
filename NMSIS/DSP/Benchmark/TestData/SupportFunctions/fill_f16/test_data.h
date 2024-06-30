#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 256

static float16_t f16_fill_value = 55.0f16;

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */