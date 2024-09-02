#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 256

float16_t rms_f16_input[ARRAY_SIZE];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */