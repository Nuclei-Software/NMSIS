#ifndef __FUNCS_DEF__
#define __FUNCS_DEF__

extern void clarke_riscv_clarke_f32();
extern void clarke_riscv_clarke_q31();

extern void inv_clarke_riscv_inv_clarke_f32();
extern void inv_clarke_riscv_inv_clarke_q31();

extern void park_riscv_park_f32();
extern void park_riscv_park_q31();

extern void inv_park_riscv_inv_park_f32();
extern void inv_park_riscv_inv_park_q31();

extern void pid_riscv_pid_f32();
extern void pid_riscv_pid_q15();
extern void pid_riscv_pid_q31();


extern void sin_cos_riscv_sin_cos_f32();
extern void sin_cos_riscv_sin_cos_q31();
#endif
