#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start TransformFunctions/cfft benchmark test:\n");

    cfft_riscv_cfft_f16();
    cfft_riscv_cfft_f32();
    cfft_riscv_cfft_q15();
    cfft_riscv_cfft_q31();

    printf("All tests are passed.\n");
    printf("test for TransformFunctions/cfft benchmark finished.\n");
}
