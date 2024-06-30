#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/InterpolationFunctions/bilinear_interp_q7/test_data.h"

BENCH_DECLARE_VAR();

void bilinear_riscv_bilinear_interp_q7(void)
{

    generate_rand_q7(q7_z_array, NUM_ROWS * NUM_COLS);

    riscv_bilinear_interp_instance_q7 S_q7 = {
        NUM_ROWS,                   /*number of Rows*/
        NUM_COLS,                   /*number of Columns*/
        q7_z_array,           /*value of y*/
    };
    q7_t bilinear_interp_q7_output;
    BENCH_START(riscv_bilinear_interp_q7);
    bilinear_interp_q7_output = riscv_bilinear_interp_q7(&S_q7, x_q31_input, y_q31_input);
    BENCH_END(riscv_bilinear_interp_q7);
}
