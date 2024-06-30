#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/InterpolationFunctions/bilinear_interp_q31/test_data.h"

BENCH_DECLARE_VAR();

void bilinear_riscv_bilinear_interp_q31(void)
{

    generate_rand_q31(q31_z_array, NUM_ROWS * NUM_COLS);

    riscv_bilinear_interp_instance_q31 S_q31 = {
        NUM_ROWS,                   /*number of Rows*/
        NUM_COLS,                   /*number of Columns*/
        q31_z_array,           /*value of y*/
    };
    q31_t bilinear_interp_q31_output;
    BENCH_START(riscv_bilinear_interp_q31);
    bilinear_interp_q31_output = riscv_bilinear_interp_q31(&S_q31, x_q31_input, y_q31_input);
    BENCH_END(riscv_bilinear_interp_q31);
}
