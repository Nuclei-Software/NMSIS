extern void cmplx_conj_riscv_cmplx_conj_f32();
extern void cmplx_conj_riscv_cmplx_conj_q15();
extern void cmplx_conj_riscv_cmplx_conj_q31();

extern void cmplx_dot_prod_riscv_cmplx_dot_prod_f32();
extern void cmplx_dot_prod_riscv_cmplx_dot_prod_q15();
extern void cmplx_dot_prod_riscv_cmplx_dot_prod_q31();

extern void cmplx_mag_riscv_cmplx_mag_f32();
extern void cmplx_mag_riscv_cmplx_mag_q15();
extern void cmplx_mag_riscv_cmplx_mag_fast_q15();
extern void cmplx_mag_riscv_cmplx_mag_q31();

extern void cmplx_mag_squared_riscv_cmplx_mag_squared_f32();
extern void cmplx_mag_squared_riscv_cmplx_mag_squared_q15();
extern void cmplx_mag_squared_riscv_cmplx_mag_squared_q31();

extern void cmplx_mult_cmplx_riscv_cmplx_mult_cmplx_f32();
extern void cmplx_mult_cmplx_riscv_cmplx_mult_cmplx_q15();
extern void cmplx_mult_cmplx_riscv_cmplx_mult_cmplx_q31();

extern void cmplx_mult_real_riscv_cmplx_mult_real_f32();
extern void cmplx_mult_real_riscv_cmplx_mult_real_q15();
extern void cmplx_mult_real_riscv_cmplx_mult_real_q31();

#if defined (RISCV_FLOAT16_SUPPORTED)
extern void cmplx_conj_riscv_cmplx_conj_f16();
extern void cmplx_dot_prod_riscv_cmplx_dot_prod_f16();
extern void cmplx_mag_riscv_cmplx_mag_f16();
extern void cmplx_mag_squared_riscv_cmplx_mag_squared_f16();
extern void cmplx_mult_cmplx_riscv_cmplx_mult_cmplx_f16();
extern void cmplx_mult_real_riscv_cmplx_mult_real_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
