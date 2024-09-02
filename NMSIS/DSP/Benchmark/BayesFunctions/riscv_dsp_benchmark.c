#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start BayesFunctions benchmark test:\n\n");

    bayes_riscv_gaussian_naive_bayes_predict_f16();
    bayes_riscv_gaussian_naive_bayes_predict_f32();

    printf("All tests are passed.\n");
    printf("\ntest for BayesFunctions benchmark finished.\n");
}
