#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start ControllerFunctions benchmark test:\n");

    clarke_riscv_clarke_f32();
    clarke_riscv_clarke_q31();

    inv_clarke_riscv_inv_clarke_f32();
    inv_clarke_riscv_inv_clarke_q31();

    park_riscv_park_f32();
    park_riscv_park_q31();

    inv_park_riscv_inv_park_f32();
    inv_park_riscv_inv_park_q31();

    pid_riscv_pid_f32();
    pid_riscv_pid_q15();
    pid_riscv_pid_q31();


    sin_cos_riscv_sin_cos_f32();
    sin_cos_riscv_sin_cos_q31();

    printf("All tests are passed.\n");
    printf("test for ControllerFunctions benchmark finished.\n");
}
