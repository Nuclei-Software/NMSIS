#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start TransformFunctions/rfft benchmark test:\n");

    rfft_riscv_rfft_f32();
    rfft_riscv_rfft_q15();
    rfft_riscv_rfft_q31();

    rfft_riscv_rfft_fast_f16();
    rfft_riscv_rfft_fast_f32();

    printf("All tests are passed.\n");
    printf("test for TransformFunctions/rfft benchmark finished.\n");
}
