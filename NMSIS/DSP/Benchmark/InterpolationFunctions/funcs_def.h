#if defined (RISCV_FLOAT16_SUPPORTED)
extern void bilinear_riscv_bilinear_interp_f16();
extern void linear_riscv_linear_interp_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

extern void bilinear_riscv_bilinear_interp_f32();
extern void bilinear_riscv_bilinear_interp_q7();
extern void bilinear_riscv_bilinear_interp_q15();
extern void bilinear_riscv_bilinear_interp_q31();

extern void linear_riscv_linear_interp_f32();
extern void linear_riscv_linear_interp_q7();
extern void linear_riscv_linear_interp_q15();
extern void linear_riscv_linear_interp_q31();

extern void spline_riscv_spline_f32();