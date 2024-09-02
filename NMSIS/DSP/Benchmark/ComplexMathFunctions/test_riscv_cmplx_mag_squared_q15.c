#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ComplexMathFunctions/cmplx_mag_squared_q15/test_data.h"

BENCH_DECLARE_VAR();

void cmplx_mag_squared_riscv_cmplx_mag_squared_q15(void)
{

    q15_t cmplx_mag_squared_q15_output[2 * ARRAY_SIZE_Q15];

    generate_rand_q15(cmplx_mag_squared_q15_input, 2 * ARRAY_SIZE_Q15);

    BENCH_START(riscv_cmplx_mag_squared_q15);
    riscv_cmplx_mag_squared_q15(cmplx_mag_squared_q15_input, cmplx_mag_squared_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_cmplx_mag_squared_q15);

    return;
}
