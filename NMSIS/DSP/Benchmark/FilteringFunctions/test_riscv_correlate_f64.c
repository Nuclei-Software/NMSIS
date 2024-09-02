#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ConvolutionFunctions/correlate_f64/test_data.h"

BENCH_DECLARE_VAR();

void correlate_riscv_correlate_f64(void)
{
#ifdef F64
    float64_t correlate_f64_output[2 * max(ARRAYA_SIZE_F64, ARRAYB_SIZE_F64)];

    generate_rand_f64(correlate_input_f64_A, ARRAYA_SIZE_F64);
    generate_rand_f64(correlate_input_f64_B, ARRAYB_SIZE_F64);

    BENCH_START(riscv_correlate_f64);
    riscv_correlate_f64(correlate_input_f64_A, ARRAYA_SIZE_F64, correlate_input_f64_B, ARRAYB_SIZE_F64,
                      correlate_f64_output);
    BENCH_END(riscv_correlate_f64);
#endif
}
