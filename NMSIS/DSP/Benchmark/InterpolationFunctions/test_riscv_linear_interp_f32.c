#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/InterpolationFunctions/linear_interp_f32/test_data.h"

BENCH_DECLARE_VAR();

void linear_riscv_linear_interp_f32(void)
{

    generate_rand_f32(f32_y_array, ARRAY_SIZE);

    riscv_linear_interp_instance_f32 S_f32 = {
        ARRAY_SIZE,       /*number of values*/
        x1,                /*start value of x*/
        xSpacing,              /*x's spacing*/
        f32_y_array,      /*value of y*/
    };
    float32_t linear_interp_f32_output;
    BENCH_START(riscv_linear_interp_f32);
    linear_interp_f32_output = riscv_linear_interp_f32(&S_f32, x_f32_input);
    BENCH_END(riscv_linear_interp_f32);
}
