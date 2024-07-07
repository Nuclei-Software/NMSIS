#include <stdio.h>
#include "riscv_math.h"
#include "../funcs_def.h"

int main()
{
    printf("Start TransformFunctions/cfft benchmark test:\n");

#if defined (RISCV_FLOAT16_SUPPORTED)
    cfft_riscv_cfft_f16();
    cfft_riscv_cfft_radix2_f16();
    cfft_riscv_cfft_radix4_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
    cfft_riscv_cfft_f32();
    cfft_riscv_cfft_q15();
    cfft_riscv_cfft_q31();
    cfft_riscv_cfft_radix2_f32();
    cfft_riscv_cfft_radix2_q15();
    cfft_riscv_cfft_radix2_q31();
    cfft_riscv_cfft_radix4_f32();
    cfft_riscv_cfft_radix4_q15();
    cfft_riscv_cfft_radix4_q31();

    printf("All tests are passed.\n");
    printf("test for TransformFunctions/cfft benchmark finished.\n");
}
