#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/InterpolationFunctions/linear_interp_q15/test_data.h"

BENCH_DECLARE_VAR();

void linear_riscv_linear_interp_q15(void)
{

    generate_rand_q15(q15_y_array, ARRAY_SIZE);

    q15_t linear_interp_q15_output;
    BENCH_START(riscv_linear_interp_q15);
    linear_interp_q15_output = riscv_linear_interp_q15(q15_y_array, x_q15_input, ARRAY_SIZE);
    BENCH_END(riscv_linear_interp_q15);
}
