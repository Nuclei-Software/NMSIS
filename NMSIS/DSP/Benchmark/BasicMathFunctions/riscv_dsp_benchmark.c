#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start BasicMathFunctions benchmark test:\n");

#if defined (RISCV_FLOAT16_SUPPORTED)
    abs_riscv_abs_f16();
    add_riscv_add_f16();
    clip_riscv_clip_f16();
    dot_prod_riscv_dot_prod_f16();
    mul_riscv_mul_f16();
    negate_riscv_negate_f16();
    offset_riscv_offset_f16();
    scale_riscv_scale_f16();
    sub_riscv_sub_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    abs_riscv_abs_f32();
    abs_riscv_abs_q7();
    abs_riscv_abs_q15();
    abs_riscv_abs_q31();

    add_riscv_add_f32();
    add_riscv_add_q7();
    add_riscv_add_q15();
    add_riscv_add_q31();

    clip_riscv_clip_f32();
    clip_riscv_clip_q7();
    clip_riscv_clip_q15();
    clip_riscv_clip_q31();

    dot_prod_riscv_dot_prod_f32();
    dot_prod_riscv_dot_prod_q7();
    dot_prod_riscv_dot_prod_q15();
    dot_prod_riscv_dot_prod_q31();

    mul_riscv_mul_f32();
    mul_riscv_mul_q7();
    mul_riscv_mul_q15();
    mul_riscv_mul_q31();

    negate_riscv_negate_f32();
    negate_riscv_negate_q7();
    negate_riscv_negate_q15();
    negate_riscv_negate_q31();

    offset_riscv_offset_f32();
    offset_riscv_offset_q7();
    offset_riscv_offset_q15();
    offset_riscv_offset_q31();

    scale_riscv_scale_f32();
    scale_riscv_scale_q7();
    scale_riscv_scale_q15();
    scale_riscv_scale_q31();

    shift_riscv_shift_q7();
    shift_riscv_shift_q15();
    shift_riscv_shift_q31();

    sub_riscv_sub_f32();
    sub_riscv_sub_q7();
    sub_riscv_sub_q15();
    sub_riscv_sub_q31();

    and_riscv_and_u8();
    and_riscv_and_u16();
    and_riscv_and_u32();

    not_riscv_not_u8();
    not_riscv_not_u16();
    not_riscv_not_u32();

    or_riscv_or_u8();
    or_riscv_or_u16();
    or_riscv_or_u32();

    xor_riscv_xor_u8();
    xor_riscv_xor_u16();
    xor_riscv_xor_u32();

    printf("All tests are passed.\n");
    printf("\ntest for BasicMathFunctions benchmark finished.\n");
}
