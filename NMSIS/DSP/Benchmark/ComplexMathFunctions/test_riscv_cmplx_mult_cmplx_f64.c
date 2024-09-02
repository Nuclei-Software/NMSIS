#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ComplexMathFunctions/cmplx_mult_cmplx_f64/test_data.h"

BENCH_DECLARE_VAR();

void cmplx_mult_cmplx_riscv_cmplx_mult_cmplx_f64(void)
{
#ifdef F64
    float64_t cmplx_mult_cmplx_f64_output[2 * ARRAY_SIZE_F64];

    generate_rand_f64(cmplx_mult_cmplx_f64_input1, 2 * ARRAY_SIZE_F64);
    generate_rand_f64(cmplx_mult_cmplx_f64_input2, 2 * ARRAY_SIZE_F64);

    BENCH_START(riscv_cmplx_mult_cmplx_f64);
    riscv_cmplx_mult_cmplx_f64(cmplx_mult_cmplx_f64_input1, cmplx_mult_cmplx_f64_input2, cmplx_mult_cmplx_f64_output, ARRAY_SIZE_F64);
    BENCH_END(riscv_cmplx_mult_cmplx_f64);
#endif
}
