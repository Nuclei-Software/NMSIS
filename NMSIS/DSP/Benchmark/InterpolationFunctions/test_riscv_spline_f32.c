#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/InterpolationFunctions/spline_f32/test_data.h"

BENCH_DECLARE_VAR();

void spline_riscv_spline_f32(void)
{
    float32_t spline_f32_output[ARRAY_SIZE];

    riscv_spline_instance_f32 S_spline;
    riscv_spline_init_f32(&S_spline, RISCV_SPLINE_NATURAL, f32_x_array, f32_y_array, FUNC_SIZE, f32_spline_coef_array, f32_spline_temp_array);
    BENCH_START(riscv_spline_f32);
    riscv_spline_f32(&S_spline, f32_test_x_array, spline_f32_output, SPLINE_FUNC_SIZE);
    BENCH_END(riscv_spline_f32);
}


