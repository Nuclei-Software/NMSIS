#ifndef __FUNCS_DEF__
#define __FUNCS_DEF__

extern void abs_riscv_abs_f16();
extern void abs_riscv_abs_f32();
extern void abs_riscv_abs_q7();
extern void abs_riscv_abs_q15();
extern void abs_riscv_abs_q31();

extern void add_riscv_add_f16();
extern void add_riscv_add_f32();
extern void add_riscv_add_q7();
extern void add_riscv_add_q15();
extern void add_riscv_add_q31();

extern void clip_riscv_clip_f16();
extern void clip_riscv_clip_f32();
extern void clip_riscv_clip_q7();
extern void clip_riscv_clip_q15();
extern void clip_riscv_clip_q31();

extern void dot_prod_riscv_dot_prod_f16();
extern void dot_prod_riscv_dot_prod_f32();
extern void dot_prod_riscv_dot_prod_q7();
extern void dot_prod_riscv_dot_prod_q15();
extern void dot_prod_riscv_dot_prod_q31();

extern void mul_riscv_mul_f16();
extern void mul_riscv_mul_f32();
extern void mul_riscv_mul_q7();
extern void mul_riscv_mul_q15();
extern void mul_riscv_mul_q31();

extern void negate_riscv_negate_f16();
extern void negate_riscv_negate_f32();
extern void negate_riscv_negate_q7();
extern void negate_riscv_negate_q15();
extern void negate_riscv_negate_q31();

extern void offset_riscv_offset_f16();
extern void offset_riscv_offset_f32();
extern void offset_riscv_offset_q7();
extern void offset_riscv_offset_q15();
extern void offset_riscv_offset_q31();

extern void scale_riscv_scale_f16();
extern void scale_riscv_scale_f32();
extern void scale_riscv_scale_q7();
extern void scale_riscv_scale_q15();
extern void scale_riscv_scale_q31();

extern void shift_riscv_shift_q7();
extern void shift_riscv_shift_q15();
extern void shift_riscv_shift_q31();

extern void sub_riscv_sub_f16();
extern void sub_riscv_sub_f32();
extern void sub_riscv_sub_q7();
extern void sub_riscv_sub_q15();
extern void sub_riscv_sub_q31();

extern void and_riscv_and_u8();
extern void and_riscv_and_u16();
extern void and_riscv_and_u32();

extern void not_riscv_not_u8();
extern void not_riscv_not_u16();
extern void not_riscv_not_u32();

extern void or_riscv_or_u8();
extern void or_riscv_or_u16();
extern void or_riscv_or_u32();

extern void xor_riscv_xor_u8();
extern void xor_riscv_xor_u16();
extern void xor_riscv_xor_u32();

#endif