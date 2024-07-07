#if defined (RISCV_FLOAT16_SUPPORTED)
extern void absmax_riscv_absmax_f16();
extern void absmaxNoIdx_riscv_absmax_no_idx_f16();
extern void absmin_riscv_absmin_f16();
extern void absminNoIdx_riscv_absmin_no_idx_f16();
extern void entropy_riscv_entropy_f16();
extern void kullbackLeibler_riscv_kullback_leibler_f16();
extern void logsumexpDotProd_riscv_logsumexp_dot_prod_f16();
extern void logsumexp_riscv_logsumexp_f16();
extern void max_riscv_max_f16();
extern void maxNoIdx_riscv_max_no_idx_f16();
extern void mean_riscv_mean_f16();
extern void min_riscv_min_f16();
extern void minNoIdx_riscv_min_no_idx_f16();
extern void mse_riscv_mse_f16();
extern void power_riscv_power_f16();
extern void rms_riscv_rms_f16();
extern void std_riscv_std_f16();
extern void var_riscv_var_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

extern void absmax_riscv_absmax_f32();
extern void absmax_riscv_absmax_q7();
extern void absmax_riscv_absmax_q15();
extern void absmax_riscv_absmax_q31();

extern void absmaxNoIdx_riscv_absmax_no_idx_f32();
extern void absmaxNoIdx_riscv_absmax_no_idx_q7();
extern void absmaxNoIdx_riscv_absmax_no_idx_q15();
extern void absmaxNoIdx_riscv_absmax_no_idx_q31();

extern void absmin_riscv_absmin_f32();
extern void absmin_riscv_absmin_q7();
extern void absmin_riscv_absmin_q15();
extern void absmin_riscv_absmin_q31();

extern void absminNoIdx_riscv_absmin_no_idx_f32();
extern void absminNoIdx_riscv_absmin_no_idx_q7();
extern void absminNoIdx_riscv_absmin_no_idx_q15();
extern void absminNoIdx_riscv_absmin_no_idx_q31();

extern void entropy_riscv_entropy_f32();

extern void kullbackLeibler_riscv_kullback_leibler_f32();

extern void logsumexpDotProd_riscv_logsumexp_dot_prod_f32();

extern void logsumexp_riscv_logsumexp_f32();

extern void max_riscv_max_f32();
extern void max_riscv_max_q7();
extern void max_riscv_max_q15();
extern void max_riscv_max_q31();

extern void maxNoIdx_riscv_max_no_idx_f32();
extern void maxNoIdx_riscv_max_no_idx_q7();
extern void maxNoIdx_riscv_max_no_idx_q15();
extern void maxNoIdx_riscv_max_no_idx_q31();

extern void mean_riscv_mean_f32();
extern void mean_riscv_mean_q7();
extern void mean_riscv_mean_q15();
extern void mean_riscv_mean_q31();

extern void min_riscv_min_f32();
extern void min_riscv_min_q7();
extern void min_riscv_min_q15();
extern void min_riscv_min_q31();

extern void minNoIdx_riscv_min_no_idx_f32();
extern void minNoIdx_riscv_min_no_idx_q7();
extern void minNoIdx_riscv_min_no_idx_q15();
extern void minNoIdx_riscv_min_no_idx_q31();

extern void mse_riscv_mse_f32();
extern void mse_riscv_mse_q7();
extern void mse_riscv_mse_q15();
extern void mse_riscv_mse_q31();

extern void power_riscv_power_f32();
extern void power_riscv_power_q7();
extern void power_riscv_power_q15();
extern void power_riscv_power_q31();

extern void rms_riscv_rms_f32();
extern void rms_riscv_rms_q15();
extern void rms_riscv_rms_q31();

extern void std_riscv_std_f32();
extern void std_riscv_std_q15();
extern void std_riscv_std_q31();

extern void var_riscv_var_f32();
extern void var_riscv_var_q15();
extern void var_riscv_var_q31();

#ifdef F64
extern void absmax_riscv_absmax_f64();
extern void absmaxNoIdx_riscv_absmax_no_idx_f64();
extern void absmin_riscv_absmin_f64();
extern void absminNoIdx_riscv_absmin_no_idx_f64();
extern void entropy_riscv_entropy_f64();
extern void kullbackLeibler_riscv_kullback_leibler_f64();
extern void max_riscv_max_f64();
extern void maxNoIdx_riscv_max_no_idx_f64();
extern void min_riscv_min_f64();
extern void minNoIdx_riscv_min_no_idx_f64();
extern void mean_riscv_mean_f64();
extern void mse_riscv_mse_f64();
extern void power_riscv_power_f64();
extern void std_riscv_std_f64();
extern void var_riscv_var_f64();
#endif
