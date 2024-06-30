#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start FastMathFunctions benchmark test:\n");

#if defined (RISCV_FLOAT16_SUPPORTED)
    vexp_riscv_vexp_f16();
    vinverse_riscv_vinverse_f16();
    vlog_riscv_vlog_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

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

    vexp_riscv_vexp_f32();

    vlog_riscv_vlog_f32();
    vlog_riscv_vlog_q15();
    vlog_riscv_vlog_q31();

#ifdef F64
    vexp_riscv_vexp_f64();
    vlog_riscv_vlog_f64();
#endif

    printf("test for FastMathFunctions benchmark finished.\n");
}
