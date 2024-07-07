#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start MatrixFunctions benchmark test:\n");

#if defined (RISCV_FLOAT16_SUPPORTED)
    matAdd_riscv_mat_add_f16();
    matCholesky_riscv_mat_cholesky_f16();

    matCmplxMult_riscv_mat_cmplx_mult_f16();
    matInverse_riscv_mat_inverse_f16();
    matMult_riscv_mat_mult_f16();
    matScale_riscv_mat_scale_f16();
    matSub_riscv_mat_sub_f16();
    matTrans_riscv_mat_trans_f16();
    matVecMult_riscv_mat_vec_mult_f16();
    matSolveLower_riscv_mat_solve_lower_triangular_f16();
    matSolveUpper_riscv_mat_solve_upper_triangular_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    matAdd_riscv_mat_add_f32();
    matAdd_riscv_mat_add_q15();
    matAdd_riscv_mat_add_q31();

    matCholesky_riscv_mat_cholesky_f32();

    matCmplxMult_riscv_mat_cmplx_mult_f32();
    matCmplxMult_riscv_mat_cmplx_mult_q15();
    matCmplxMult_riscv_mat_cmplx_mult_q31();

    matInverse_riscv_mat_inverse_f32();

    matLdlt_riscv_mat_ldlt_f32();

    matMult_riscv_mat_mult_f32();
    matMult_riscv_mat_mult_q7();
    matMult_riscv_mat_mult_q15();
    matMultFast_riscv_mat_mult_fast_q15();
    matMult_riscv_mat_mult_q31();
    matMultFast_riscv_mat_mult_fast_q31();
    matMultOpt_riscv_mat_mult_opt_q31();

    matScale_riscv_mat_scale_f32();
    matScale_riscv_mat_scale_q15();
    matScale_riscv_mat_scale_q31();

    matSub_riscv_mat_sub_f32();
    matSub_riscv_mat_sub_q15();
    matSub_riscv_mat_sub_q31();

    matTrans_riscv_mat_trans_f32();
    matTrans_riscv_mat_trans_q7();
    matTrans_riscv_mat_trans_q15();
    matTrans_riscv_mat_trans_q31();

    matVecMult_riscv_mat_vec_mult_f32();
    matVecMult_riscv_mat_vec_mult_q7();
    matVecMult_riscv_mat_vec_mult_q15();
    matVecMult_riscv_mat_vec_mult_q31();

    matSolveLower_riscv_mat_solve_lower_triangular_f32();
    matSolveUpper_riscv_mat_solve_upper_triangular_f32();

    printf("All tests are passed.\n");
    printf("test for MatrixFunctions benchmark finished.\n");
}
