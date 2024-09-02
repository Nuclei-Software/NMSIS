#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/vlog_f64/test_data.h"

BENCH_DECLARE_VAR();

void vlog_riscv_vlog_f64(void)
{
#ifdef F64
    float64_t vlog_f64_output[ARRAY_SIZE_F64];

    generate_rand_f64(vlog_f64_input, ARRAY_SIZE_F64);

    BENCH_START(riscv_vlog_f64);
    riscv_vlog_f64(vlog_f64_input, vlog_f64_output, ARRAY_SIZE_F64);
    BENCH_END(riscv_vlog_f64);
#endif
}