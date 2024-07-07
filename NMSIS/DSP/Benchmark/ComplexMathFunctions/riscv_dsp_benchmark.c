#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start ComplexMathFunctions benchmark test:\n");

    cmplx_conj_riscv_cmplx_conj_f32();
    cmplx_conj_riscv_cmplx_conj_q15();
    cmplx_conj_riscv_cmplx_conj_q31();

    cmplx_dot_prod_riscv_cmplx_dot_prod_f32();
    cmplx_dot_prod_riscv_cmplx_dot_prod_q15();
    cmplx_dot_prod_riscv_cmplx_dot_prod_q31();

    cmplx_mag_riscv_cmplx_mag_f32();
    cmplx_mag_riscv_cmplx_mag_q15();
    cmplx_mag_riscv_cmplx_mag_q31();
    cmplx_mag_riscv_cmplx_mag_fast_q15();

    cmplx_mag_squared_riscv_cmplx_mag_squared_f32();
    cmplx_mag_squared_riscv_cmplx_mag_squared_q15();
    cmplx_mag_squared_riscv_cmplx_mag_squared_q31();

    cmplx_mult_cmplx_riscv_cmplx_mult_cmplx_f32();
    cmplx_mult_cmplx_riscv_cmplx_mult_cmplx_q15();
    cmplx_mult_cmplx_riscv_cmplx_mult_cmplx_q31();

    cmplx_mult_real_riscv_cmplx_mult_real_f32();
    cmplx_mult_real_riscv_cmplx_mult_real_q15();
    cmplx_mult_real_riscv_cmplx_mult_real_q31();

#if defined (RISCV_FLOAT16_SUPPORTED)
    cmplx_conj_riscv_cmplx_conj_f16();
    cmplx_dot_prod_riscv_cmplx_dot_prod_f16();
    cmplx_mag_riscv_cmplx_mag_f16();
    cmplx_mag_squared_riscv_cmplx_mag_squared_f16();
    cmplx_mult_cmplx_riscv_cmplx_mult_cmplx_f16();
    cmplx_mult_real_riscv_cmplx_mult_real_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    printf("All tests are passed.\n");
    printf("test for ComplexMathFunctions benchmark finished.\n");
}
