#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start InterpolationFunctions benchmark test:\n");

#if defined (RISCV_FLOAT16_SUPPORTED)
    bilinear_riscv_bilinear_interp_f16();
    linear_riscv_linear_interp_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    bilinear_riscv_bilinear_interp_f32();
    bilinear_riscv_bilinear_interp_q7();
    bilinear_riscv_bilinear_interp_q15();
    bilinear_riscv_bilinear_interp_q31();

    linear_riscv_linear_interp_f32();
    linear_riscv_linear_interp_q7();
    linear_riscv_linear_interp_q15();
    linear_riscv_linear_interp_q31();

    spline_riscv_spline_f32();

    printf("All tests are passed.\n");
    printf("test for InterpolationFunctions benchmark finished.\n");
}
