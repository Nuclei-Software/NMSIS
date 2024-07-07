#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/InterpolationFunctions/linear_interp_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void linear_riscv_linear_interp_f16(void)
{

    generate_rand_f16(f16_y_array, ARRAY_SIZE);

    riscv_linear_interp_instance_f16 S_f16 = {
        ARRAY_SIZE,       /*number of values*/
        x1,                /*start value of x*/
        xSpacing,              /*x's spacing*/
        f16_y_array,      /*value of y*/
    };
    float16_t linear_interp_f16_output;
    BENCH_START(riscv_linear_interp_f16);
    linear_interp_f16_output = riscv_linear_interp_f16(&S_f16, x_f16_input);
    BENCH_END(riscv_linear_interp_f16);
}

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
