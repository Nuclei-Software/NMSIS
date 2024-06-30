#pragma once
#include <stdint.h>
#include "riscv_math.h"
#include "../bayes_f32/test_data.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

static float16_t pBufferB_f16[NB_OF_CLASSES] = {};

static float16_t bayes_f16_input[VECTOR_DIMENSION] = {};

static float16_t theta_f16[NB_OF_CLASSES * VECTOR_DIMENSION] = {};

static float16_t sigma_f16[NB_OF_CLASSES * VECTOR_DIMENSION] = {};

static float16_t classPriors_f16[NB_OF_CLASSES]= {};

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
