#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start BayesFunctions benchmark test:\n\n");

#if defined (RISCV_FLOAT16_SUPPORTED)
    bayes_riscv_gaussian_naive_bayes_predict_f16();
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */

    bayes_riscv_gaussian_naive_bayes_predict_f32();

    printf("\ntest for BayesFunctions benchmark finished.\n");
}
