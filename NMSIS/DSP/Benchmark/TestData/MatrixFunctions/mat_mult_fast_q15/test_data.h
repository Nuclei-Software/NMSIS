#pragma once
#include <stdint.h>


#define M 8
#define K 256
#define N 128

static q15_t mat_mult_q15_arrayA[M * K];
static q15_t mat_mult_q15_arrayB[K * N];
