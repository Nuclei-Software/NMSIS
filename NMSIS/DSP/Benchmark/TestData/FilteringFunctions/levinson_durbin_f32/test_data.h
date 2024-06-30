#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define COEFSIZE  180

static float32_t phi[COEFSIZE + 1] = {};
static float32_t autoRegreCoef[COEFSIZE] = {};
