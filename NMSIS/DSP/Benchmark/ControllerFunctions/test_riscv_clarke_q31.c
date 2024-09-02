#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ControllerFunctions/clarke_q31/test_data.h"

BENCH_DECLARE_VAR();

void clarke_riscv_clarke_q31(void)
{
    q31_t pIalpha_q31[ARRAY_SIZE_Q31];
    q31_t pIbeta_q31[ARRAY_SIZE_Q31];

    generate_rand_q31(Ia_q31, ARRAY_SIZE_Q31);
    generate_rand_q31(Ib_q31, ARRAY_SIZE_Q31);

    volatile int i = 0;

    BENCH_START(riscv_clarke_q31);
    for (i = 0; i < ARRAY_SIZE_Q31; i++) {
        riscv_clarke_q31(Ia_q31[i], Ib_q31[i], &pIalpha_q31[i], &pIbeta_q31[i]);
    }
    BENCH_END(riscv_clarke_q31);

    return;
}