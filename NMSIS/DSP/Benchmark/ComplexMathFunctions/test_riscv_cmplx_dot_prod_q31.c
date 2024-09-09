#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ComplexMathFunctions/cmplx_dot_prod_q31/test_data.h"

BENCH_DECLARE_VAR();

void cmplx_dot_prod_riscv_cmplx_dot_prod_q31(void)
{

    q63_t cmplx_dot_prod_q63_realOutput, cmplx_dot_prod_q63_imagOutput;

    generate_rand_q31(cmplx_dot_prod_q31_input1, 2 * ARRAY_SIZE_Q31);
    generate_rand_q31(cmplx_dot_prod_q31_input2, 2 * ARRAY_SIZE_Q31);

    BENCH_START(riscv_cmplx_dot_prod_q31);
    riscv_cmplx_dot_prod_q31(cmplx_dot_prod_q31_input1, cmplx_dot_prod_q31_input2, ARRAY_SIZE_Q31, &cmplx_dot_prod_q63_realOutput,
                           &cmplx_dot_prod_q63_imagOutput);
    BENCH_END(riscv_cmplx_dot_prod_q31);

    return;
}
