#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ComplexMathFunctions/cmplx_mag_squared_f64/test_data.h"

BENCH_DECLARE_VAR();

void cmplx_mag_squared_riscv_cmplx_mag_squared_f64(void)
{

    float64_t cmplx_mag_squared_f64_output[2 * ARRAY_SIZE_F64];

    generate_rand_f64(cmplx_mag_squared_f64_input, 2 * ARRAY_SIZE_F64);

    BENCH_START(riscv_cmplx_mag_squared_f64);
    riscv_cmplx_mag_squared_f64(cmplx_mag_squared_f64_input, cmplx_mag_squared_f64_output, ARRAY_SIZE_F64);
    BENCH_END(riscv_cmplx_mag_squared_f64);

    return;
}
