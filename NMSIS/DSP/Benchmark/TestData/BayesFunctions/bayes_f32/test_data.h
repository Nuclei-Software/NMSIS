#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define NB_OF_CLASSES 3
#define VECTOR_DIMENSION 2

static float32_t pBufferB_f32[NB_OF_CLASSES] = {};

static float32_t bayes_f32_input[VECTOR_DIMENSION] = {};

static float32_t epsilon_f32 = 4.731813637210494e-09f;

static const float32_t theta_f32[NB_OF_CLASSES * VECTOR_DIMENSION] = {
    1.525400412246743, 1.1286029390034396, -1.5547934054920127, 0.9976789084710983, -0.14944114212609047, -3.0461975645866857}; /**< Mean values for the Gaussians */

static const float32_t sigma_f32[NB_OF_CLASSES * VECTOR_DIMENSION] = {
    0.8790413342380022, 1.0687689375338891, 0.9512598687040021, 0.9514543522981638, 1.0755900156857314, 0.9088718773623725}; /**< Variances for the Gaussians */

static const float32_t classPriors_f32[NB_OF_CLASSES] = {
    0.3333333333333333f, 0.3333333333333333f, 0.3333333333333333f}; /**< Class prior probabilities */
