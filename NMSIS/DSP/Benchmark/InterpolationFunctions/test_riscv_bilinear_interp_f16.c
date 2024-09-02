#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/InterpolationFunctions/bilinear_interp_f16/test_data.h"

BENCH_DECLARE_VAR();

void bilinear_riscv_bilinear_interp_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    generate_rand_f16(f16_z_array, NUM_ROWS * NUM_COLS);

    riscv_bilinear_interp_instance_f16 S_f16 = {
        NUM_ROWS,                 /*number of Rows*/
        NUM_COLS,                 /*number of Columns*/
        f16_z_array,        /*value of y*/
    };

    float16_t bilinear_interp_f16_output;

    BENCH_START(riscv_bilinear_interp_f16);
    bilinear_interp_f16_output = riscv_bilinear_interp_f16(&S_f16, x_f16_input, y_f16_input);
    BENCH_END(riscv_bilinear_interp_f16);
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
}

