#pragma once
#include <stdint.h>


#define VECTOR_DIMENSION 256
#define NB_SUPPORT_VECTORS 2

static float32_t intercept = 0.116755f;
static float32_t f32_input[VECTOR_DIMENSION] = {};
static float32_t dualCoefficients[NB_SUPPORT_VECTORS]={}; /* Dual coefficients */
static float32_t supportVectors[NB_SUPPORT_VECTORS * VECTOR_DIMENSION]={}; /* Support vectors */
static int32_t classes[2] = {0, 1};