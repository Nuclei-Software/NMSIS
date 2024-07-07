#pragma once
#include <stdint.h>
#include "riscv_math.h"

#define COEFSIZE  256

static q31_t phi[COEFSIZE + 1] = {};
static q31_t autoRegreCoef[COEFSIZE] = {};
