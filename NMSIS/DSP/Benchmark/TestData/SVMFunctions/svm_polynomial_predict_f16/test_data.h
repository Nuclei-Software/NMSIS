#pragma once
#include <stdint.h>
#include "riscv_math.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

#define NB_SUPPORT_VECTORS 2
#define VECTOR_DIMENSION 256

static int32_t degree = 3;
static float16_t coef0_f16 = (float16_t)1.100000f;
static float16_t gamma_f16 = (float16_t)0.500000f;
static float16_t intercept = (float16_t)-1.704476f;
static float16_t f16_input[VECTOR_DIMENSION] = {};
static float16_t dualCoefficients[NB_SUPPORT_VECTORS]={}; /* Dual coefficients */
static float16_t supportVectors[NB_SUPPORT_VECTORS * VECTOR_DIMENSION]={}; /* Support vectors */
static const int32_t classes[2] = {0, 1};

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
