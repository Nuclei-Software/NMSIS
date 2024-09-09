#if defined (RISCV_FLOAT16_SUPPORTED)
extern void vexp_riscv_vexp_f16();
extern void vinverse_riscv_vinverse_f16();
extern void vlog_riscv_vlog_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

extern void cos_riscv_cos_f32();
extern void cos_riscv_cos_q15();
extern void cos_riscv_cos_q31();

extern void divide_riscv_divide_q15();
extern void divide_riscv_divide_q31();

extern void sin_riscv_sin_f32();
extern void sin_riscv_sin_q15();
extern void sin_riscv_sin_q31();

extern void sqrt_riscv_sqrt_f32();
extern void sqrt_riscv_sqrt_q15();
extern void sqrt_riscv_sqrt_q31();

extern void vexp_riscv_vexp_f32();

extern void vlog_riscv_vlog_f32();
extern void vlog_riscv_vlog_q15();
extern void vlog_riscv_vlog_q31();

#ifdef F64
extern void vexp_riscv_vexp_f64();
extern void vlog_riscv_vlog_f64();
#endif