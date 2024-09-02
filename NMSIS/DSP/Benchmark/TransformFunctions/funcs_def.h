#ifndef __FUNCS_DEF__
#define __FUNCS_DEF__

extern void cfft_riscv_cfft_f16();
extern void cfft_riscv_cfft_f32();
extern void cfft_riscv_cfft_q15();
extern void cfft_riscv_cfft_q31();

extern void cfft_riscv_cfft_radix2_f16();
extern void cfft_riscv_cfft_radix2_f32();
extern void cfft_riscv_cfft_radix2_q15();
extern void cfft_riscv_cfft_radix2_q31();

extern void cfft_riscv_cfft_radix4_f16();
extern void cfft_riscv_cfft_radix4_f32();
extern void cfft_riscv_cfft_radix4_q15();
extern void cfft_riscv_cfft_radix4_q31();

extern void dct4_riscv_dct4_f32();
extern void dct4_riscv_dct4_q15();
extern void dct4_riscv_dct4_q31();

extern void rfft_riscv_rfft_f32();
extern void rfft_riscv_rfft_q15();
extern void rfft_riscv_rfft_q31();
extern void rfft_riscv_rfft_fast_f16();
extern void rfft_riscv_rfft_fast_f32();

#endif
