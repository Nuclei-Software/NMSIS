#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE_F16 512

static float16_t cmplx_mag_f16_input[2 * ARRAY_SIZE_F16] = {};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
