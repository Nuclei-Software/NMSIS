#pragma once
#include <stdint.h>


#define ARRAYA_SIZE_Q7 1024
#define ARRAYB_SIZE_Q7 512

static q7_t test_conv_input_q7_A[ARRAYA_SIZE_Q7];
static q7_t test_conv_input_q7_B[ARRAYB_SIZE_Q7];
static q15_t pScratch1[max(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7) + 2 * min(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7) - 2];
static q15_t pScratch2[min(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7)];
