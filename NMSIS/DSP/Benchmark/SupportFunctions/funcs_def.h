#ifndef __FUNCS_DEF__
#define __FUNCS_DEF__

extern void barycenter_riscv_barycenter_f16();
extern void barycenter_riscv_barycenter_f32();

extern void copy_riscv_copy_f16();
extern void copy_riscv_copy_f32();
extern void copy_riscv_copy_f64();
extern void copy_riscv_copy_q7();
extern void copy_riscv_copy_q15();
extern void copy_riscv_copy_q31();

extern void fill_riscv_fill_f16();
extern void fill_riscv_fill_f32();
extern void fill_riscv_fill_f64();
extern void fill_riscv_fill_q7();
extern void fill_riscv_fill_q15();
extern void fill_riscv_fill_q31();

extern void f16ToF32_riscv_f16_to_float();
extern void f16ToF64_riscv_f16_to_f64();
extern void f16ToQ15_riscv_f16_to_q15();

extern void f32ToF16_riscv_float_to_f16();
extern void f32ToF64_riscv_float_to_f64();
extern void f32ToQ7_riscv_float_to_q7();
extern void f32ToQ15_riscv_float_to_q15();
extern void f32ToQ31_riscv_float_to_q31();

extern void f64ToF16_riscv_f64_to_f16();
extern void f64ToF32_riscv_f64_to_float();
extern void f64Toq7_riscv_f64_to_q7();
extern void f64Toq15_riscv_f64_to_q15();
extern void f64Toq31_riscv_f64_to_q31();

extern void q7ToF32_riscv_q7_to_float();
extern void q7ToF64_riscv_q7_to_f64();
extern void q7ToQ15_riscv_q7_to_q15();
extern void q7ToQ31_riscv_q7_to_q31();

extern void q15ToF16_riscv_q15_to_f16();
extern void q15ToF32_riscv_q15_to_f32();
extern void q15ToF64_riscv_q15_to_f64();
extern void q15ToQ7_riscv_q15_to_q7();
extern void q15ToQ31_riscv_q15_to_q31();

extern void q31ToF64_riscv_q31_to_f64();
extern void q31ToF32_riscv_q31_to_f32();
extern void q31ToQ7_riscv_q31_to_q7();
extern void q31ToQ15_riscv_q31_to_q15();

extern void sort_riscv_sort_f32();
extern void mergeSort_riscv_merge_sort_f32();

extern void weightedSum_riscv_weighted_sum_f16();
extern void weightedSum_riscv_weighted_sum_f32();

#endif
