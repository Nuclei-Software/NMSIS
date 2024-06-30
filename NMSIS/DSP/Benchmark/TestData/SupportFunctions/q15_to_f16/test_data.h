#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 128

static q15_t q15_input[ARRAY_SIZE] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */