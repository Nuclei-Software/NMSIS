#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ControllerFunctions/sin_cos_q31/test_data.h"

BENCH_DECLARE_VAR();

void sin_cos_riscv_sin_cos_q31(void)
{
    volatile int i;
    q31_t pSinVal_q31[ARRAY_SIZE_Q31];
    q31_t pCosVal_q31[ARRAY_SIZE_Q31];

    for (i = 0; i < ARRAY_SIZE_Q31; i++) {
        sin_cos_q31_input[i] = (float32_t)rand() / 0x7ff;
    }

    BENCH_START(riscv_sin_cos_q31);
    for (i = 0; i < ARRAY_SIZE_Q31; i++) {
        riscv_sin_cos_q31(sin_cos_q31_input[i], &pSinVal_q31[i], &pCosVal_q31[i]);
    }
    BENCH_END(riscv_sin_cos_q31);
}
