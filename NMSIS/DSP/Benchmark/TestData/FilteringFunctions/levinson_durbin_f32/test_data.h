#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define COEFSIZE  256

static float32_t phi[COEFSIZE + 1] = {};
static float32_t autoRegreCoef[COEFSIZE] = {};
