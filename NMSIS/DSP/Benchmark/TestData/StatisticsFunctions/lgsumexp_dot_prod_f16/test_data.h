#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define ARRAY_SIZE 512

float16_t logsumexp_dotProd_f16_input1[ARRAY_SIZE];

float16_t logsumexp_dotProd_f16_input2[ARRAY_SIZE];

float16_t f16_temp_array[ARRAY_SIZE];

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
