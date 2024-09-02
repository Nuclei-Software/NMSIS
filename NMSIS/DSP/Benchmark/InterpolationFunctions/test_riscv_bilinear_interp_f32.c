#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/InterpolationFunctions/bilinear_interp_f32/test_data.h"

BENCH_DECLARE_VAR();

void bilinear_riscv_bilinear_interp_f32(void)
{
    generate_rand_f32(f32_z_array, NUM_ROWS * NUM_COLS);

    riscv_bilinear_interp_instance_f32 S_f32 = {
        NUM_ROWS,                 /*number of Rows*/
        NUM_COLS,                 /*number of Columns*/
        f32_z_array,        /*value of y*/
    };

    float32_t bilinear_interp_f32_output;

    BENCH_START(riscv_bilinear_interp_f32);
    bilinear_interp_f32_output = riscv_bilinear_interp_f32(&S_f32, x_f32_input, y_f32_input);
    BENCH_END(riscv_bilinear_interp_f32);
}