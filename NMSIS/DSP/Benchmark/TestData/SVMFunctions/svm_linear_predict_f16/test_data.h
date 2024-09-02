#pragma once
#include <stdint.h>


#if defined (RISCV_FLOAT16_SUPPORTED)

#define VECTOR_DIMENSION 512
#define NB_SUPPORT_VECTORS 2

static float32_t intercept = (float16_t)0.116755f;
static float16_t f16_input[VECTOR_DIMENSION] = {};
static float16_t dualCoefficients[NB_SUPPORT_VECTORS]={}; /* Dual coefficients */
static float16_t supportVectors[NB_SUPPORT_VECTORS * VECTOR_DIMENSION]={}; /* Support vectors */
static int32_t classes[2] = {0, 1};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
