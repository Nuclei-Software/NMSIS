#if defined (RISCV_FLOAT16_SUPPORTED)
extern void matAdd_riscv_mat_add_f16();
extern void matCholesky_riscv_mat_cholesky_f16();
extern void matCmplxMult_riscv_mat_cmplx_mult_f16();
extern void matInverse_riscv_mat_inverse_f16();
extern void matMult_riscv_mat_mult_f16();
extern void matScale_riscv_mat_scale_f16();
extern void matSub_riscv_mat_sub_f16();
extern void matTrans_riscv_mat_trans_f16();
extern void matVecMult_riscv_mat_vec_mult_f16();
extern void matSolveLower_riscv_mat_solve_lower_triangular_f16();
extern void matSolveUpper_riscv_mat_solve_upper_triangular_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

extern void matAdd_riscv_mat_add_f32();
extern void matAdd_riscv_mat_add_q15();
extern void matAdd_riscv_mat_add_q31();

extern void matCholesky_riscv_mat_cholesky_f32();

extern void matCmplxMult_riscv_mat_cmplx_mult_f32();
extern void matCmplxMult_riscv_mat_cmplx_mult_q15();
extern void matCmplxMult_riscv_mat_cmplx_mult_q31();

extern void matInverse_riscv_mat_inverse_f32();

extern void matLdlt_riscv_mat_ldlt_f32();

extern void matMult_riscv_mat_mult_f32();
extern void matMult_riscv_mat_mult_q7();
extern void matMult_riscv_mat_mult_q15();
extern void matMult_riscv_mat_mult_q31();

extern void matMultFast_riscv_mat_mult_fast_q15();
extern void matMultFast_riscv_mat_mult_fast_q31();
extern void matMultOpt_riscv_mat_mult_opt_q31();

extern void matScale_riscv_mat_scale_f32();
extern void matScale_riscv_mat_scale_q15();
extern void matScale_riscv_mat_scale_q31();

extern void matSub_riscv_mat_sub_f32();
extern void matSub_riscv_mat_sub_q15();
extern void matSub_riscv_mat_sub_q31();

extern void matTrans_riscv_mat_trans_f32();
extern void matTrans_riscv_mat_trans_q7();
extern void matTrans_riscv_mat_trans_q15();
extern void matTrans_riscv_mat_trans_q31();

extern void matVecMult_riscv_mat_vec_mult_f32();
extern void matVecMult_riscv_mat_vec_mult_q7();
extern void matVecMult_riscv_mat_vec_mult_q15();
extern void matVecMult_riscv_mat_vec_mult_q31();

extern void matSolveLower_riscv_mat_solve_lower_triangular_f32();
extern void matSolveUpper_riscv_mat_solve_upper_triangular_f32();
