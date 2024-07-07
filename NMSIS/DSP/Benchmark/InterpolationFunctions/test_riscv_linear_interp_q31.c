#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/InterpolationFunctions/linear_interp_q31/test_data.h"

BENCH_DECLARE_VAR();

void linear_riscv_linear_interp_q31(void)
{

    generate_rand_q31(q31_y_array, ARRAY_SIZE);

    q31_t linear_interp_q31_output;
    BENCH_START(riscv_linear_interp_q31);
    linear_interp_q31_output = riscv_linear_interp_q31(q31_y_array, x_q31_input, ARRAY_SIZE);
    BENCH_END(riscv_linear_interp_q31);
}
