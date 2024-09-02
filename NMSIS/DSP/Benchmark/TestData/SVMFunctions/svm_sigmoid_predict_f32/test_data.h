#pragma once
#include <stdint.h>


#define NB_SUPPORT_VECTORS 2
#define VECTOR_DIMENSION 256

static float32_t gamma_f32 = 0.500000f;
static float32_t coef0_f32 = 1.100000f;
static float32_t intercept = 0.000017f;
static float32_t f32_input[VECTOR_DIMENSION] = {};
static float32_t dualCoefficients[NB_SUPPORT_VECTORS]={}; /* Dual coefficients */
static float32_t supportVectors[NB_SUPPORT_VECTORS * VECTOR_DIMENSION]={}; /* Support vectors */
static const int32_t classes[2] = {0, 1};
