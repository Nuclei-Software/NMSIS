#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ComplexMathFunctions/cmplx_dot_prod_q15/test_data.h"

BENCH_DECLARE_VAR();

void cmplx_dot_prod_riscv_cmplx_dot_prod_q15(void)
{

    q31_t cmplx_dot_prod_q31_realOutput, cmplx_dot_prod_q31_imagOutput;

    generate_rand_q15(cmplx_dot_prod_q15_input1, 2 * ARRAY_SIZE_Q15);
    generate_rand_q15(cmplx_dot_prod_q15_input2, 2 * ARRAY_SIZE_Q15);

    BENCH_START(riscv_cmplx_dot_prod_q15);
    riscv_cmplx_dot_prod_q15(cmplx_dot_prod_q15_input1, cmplx_dot_prod_q15_input2, ARRAY_SIZE_Q15, &cmplx_dot_prod_q31_realOutput,
                           &cmplx_dot_prod_q31_imagOutput);
    BENCH_END(riscv_cmplx_dot_prod_q15);

    return;
}
