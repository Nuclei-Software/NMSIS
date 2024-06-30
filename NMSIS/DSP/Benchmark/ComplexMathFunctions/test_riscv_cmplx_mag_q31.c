#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ComplexMathFunctions/cmplx_mag_q31/test_data.h"

BENCH_DECLARE_VAR();

void cmplx_mag_riscv_cmplx_mag_q31(void)
{
    q31_t cmplx_mag_q31_output[ARRAY_SIZE_Q31];

    generate_rand_q31(cmplx_mag_q31_input, ARRAY_SIZE_Q31);

    BENCH_START(riscv_cmplx_mag_q31);
    riscv_cmplx_mag_q31(cmplx_mag_q31_input, cmplx_mag_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_cmplx_mag_q31);

    return;
}
