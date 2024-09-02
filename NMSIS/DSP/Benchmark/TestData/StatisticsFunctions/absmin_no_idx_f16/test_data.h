#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 512

float16_t absmin_no_idx_input[ARRAY_SIZE];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
