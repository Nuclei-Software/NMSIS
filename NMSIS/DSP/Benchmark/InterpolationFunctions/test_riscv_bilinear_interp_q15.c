#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/InterpolationFunctions/bilinear_interp_q15/test_data.h"

BENCH_DECLARE_VAR();

void bilinear_riscv_bilinear_interp_q15(void)
{
    generate_rand_q15(q15_z_array, NUM_ROWS * NUM_COLS);

    riscv_bilinear_interp_instance_q15 S_q15 = {
        NUM_ROWS,                   /*number of Rows*/
        NUM_COLS,                   /*number of Columns*/
        q15_z_array,           /*value of y*/
    };

    q15_t bilinear_interp_q15_output;

    BENCH_START(riscv_bilinear_interp_q15);
    bilinear_interp_q15_output = riscv_bilinear_interp_q15(&S_q15, x_q31_input, y_q31_input);
    BENCH_END(riscv_bilinear_interp_q15);
}
