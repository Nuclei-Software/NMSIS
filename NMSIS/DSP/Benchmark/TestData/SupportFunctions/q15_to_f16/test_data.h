#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 512

static q15_t q15_input[ARRAY_SIZE] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */