#pragma once
#include <stdint.h>


#define M 16
#define K 128
#define N 16

static q31_t mat_mult_q31_arrayA[M * K];
static q31_t mat_mult_q31_arrayB[K * N];
