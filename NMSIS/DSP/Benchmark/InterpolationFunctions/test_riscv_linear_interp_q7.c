#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/InterpolationFunctions/linear_interp_q7/test_data.h"

BENCH_DECLARE_VAR();

void linear_riscv_linear_interp_q7(void)
{
    generate_rand_q7(y_q7_input, ARRAY_SIZE);

    q7_t linear_interp_q7_output;

    BENCH_START(riscv_linear_interp_q7);
    linear_interp_q7_output = riscv_linear_interp_q7(y_q7_input, x_q31_input, ARRAY_SIZE);
    BENCH_END(riscv_linear_interp_q7);
}
