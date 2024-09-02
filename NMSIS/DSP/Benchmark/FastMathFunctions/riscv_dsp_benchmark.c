#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start FastMathFunctions benchmark test:\n");

    vinverse_riscv_vinverse_f16();

    atan2_riscv_atan2_f16();
    atan2_riscv_atan2_f32();
    atan2_riscv_atan2_q15();
    atan2_riscv_atan2_q31();

    cos_riscv_cos_f32();
    cos_riscv_cos_q15();
    cos_riscv_cos_q31();

    divide_riscv_divide_q15();
    divide_riscv_divide_q31();

    sin_riscv_sin_f32();
    sin_riscv_sin_q15();
    sin_riscv_sin_q31();

    sqrt_riscv_sqrt_f32();
    sqrt_riscv_sqrt_q15();
    sqrt_riscv_sqrt_q31();

    vexp_riscv_vexp_f16();
    vexp_riscv_vexp_f32();
    vexp_riscv_vexp_f64();

    vlog_riscv_vlog_f16();
    vlog_riscv_vlog_f32();
    vlog_riscv_vlog_f64();
    vlog_riscv_vlog_q15();
    vlog_riscv_vlog_q31();

    printf("All tests are passed.\n");
    printf("test for FastMathFunctions benchmark finished.\n");
}
