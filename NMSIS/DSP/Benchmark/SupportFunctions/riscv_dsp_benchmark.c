#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start SupportFunctions benchmark test:\n");

    barycenter_riscv_barycenter_f16();
    barycenter_riscv_barycenter_f32();

    copy_riscv_copy_f16();
    copy_riscv_copy_f32();
    copy_riscv_copy_f64();
    copy_riscv_copy_q7();
    copy_riscv_copy_q15();
    copy_riscv_copy_q31();

    fill_riscv_fill_f16();
    fill_riscv_fill_f32();
    fill_riscv_fill_f64();
    fill_riscv_fill_q7();
    fill_riscv_fill_q15();
    fill_riscv_fill_q31();

    f16ToF32_riscv_f16_to_float();
    f16ToF64_riscv_f16_to_f64();
    f16ToQ15_riscv_f16_to_q15();

    f32ToF16_riscv_float_to_f16();
    f32ToF64_riscv_float_to_f64();
    f32ToQ7_riscv_float_to_q7();
    f32ToQ15_riscv_float_to_q15();
    f32ToQ31_riscv_float_to_q31();

    f64ToF16_riscv_f64_to_f16();
    f64ToF32_riscv_f64_to_float();
    f64Toq7_riscv_f64_to_q7();
    f64Toq15_riscv_f64_to_q15();
    f64Toq31_riscv_f64_to_q31();

    q7ToF32_riscv_q7_to_float();
    q7ToF64_riscv_q7_to_f64();
    q7ToQ15_riscv_q7_to_q15();
    q7ToQ31_riscv_q7_to_q31();

    q15ToF16_riscv_q15_to_f16();
    q15ToF32_riscv_q15_to_f32();
    q15ToF64_riscv_q15_to_f64();
    q15ToQ7_riscv_q15_to_q7();
    q15ToQ31_riscv_q15_to_q31();

    q31ToF64_riscv_q31_to_f64();
    q31ToF32_riscv_q31_to_f32();
    q31ToQ7_riscv_q31_to_q7();
    q31ToQ15_riscv_q31_to_q15();

    sort_riscv_sort_f32();
    mergeSort_riscv_merge_sort_f32();

    weightedSum_riscv_weighted_sum_f16();
    weightedSum_riscv_weighted_sum_f32();

    printf("All tests are passed.\n");
    printf("test for SupportFunctions benchmark finished.\n");
}
