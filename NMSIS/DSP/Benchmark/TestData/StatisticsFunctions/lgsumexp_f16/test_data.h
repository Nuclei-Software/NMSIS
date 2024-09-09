#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 513

float16_t logsumexp_f16_input[ARRAY_SIZE];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
